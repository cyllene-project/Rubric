// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)
#include "TwitterApp.h"
#include "tweets.h"
#include <fstream>

using namespace model;
using namespace rxcurl;
using namespace tweets;
using namespace rubric;
using namespace rubric::core;
using namespace rxcpp;
using namespace rxcpp::rxo;
using namespace rxcpp::rxs;


TwitterApp::TwitterApp(const std::string & id) : Application(id){

    // add settings

    subjects::subject<int> framebus;
    auto frameout = framebus.get_subscriber();
    auto sendframe = [=]() {
        frameout.on_next(1);
    };

    auto frames = framebus.get_observable();

    auto & mainLoop = rubric::draw::Context::getInstance().getRunLoop();

    auto mainthreadid = mainLoop.getMainThreadId();

    auto mainthread = mainLoop.getMainThread();

    auto tweetthread = observe_on_new_thread();

    auto factory = create_rxcurl();

    auto poolthread = mainLoop.getEventLoop();

    composite_subscription lifetime;

    // ==== Constants - paths
    const string URL = "https://stream.twitter.com/1.1/statuses/";

    std::string settingsFile("/settings.json");

    rxsub::replay<json, decltype(mainthread)> setting_update(1, mainthread, lifetime);
    auto setting_updates = setting_update.get_observable();
    auto sendsettings = setting_update.get_subscriber();
    auto sf = settingsFile;
    // call update_settings() to save changes and trigger parties interested in the change
    auto update_settings = [sendsettings, sf](json s){
        ofstream o(sf);
        o << setw(4) << s;
        if (sendsettings.is_subscribed()){
            sendsettings.on_next(s);
        }
    };

    json settings;

    // initial update
    update_settings(settings);

    /* filter settings updates to changes that change the url for the twitter stream.

   distinct_until_changed is used to filter out settings updates that do not change the url

   debounce is used to wait until the updates pause before signaling the url has changed.
     this is important since typing in keywords would cause many intermediate changes to the url
     and twitter rate limits the user if there are too many fast restarts.
*/
    auto urlchanges = setting_updates |
          rxo::map([=](const json& settings){
              string url = URL + settings["Query"]["Action"].get<std::string>() + ".json?";
              if (settings.count("Language") > 0) {
                  url += "language=" + settings["Language"].get<std::string>() + "&";
              }
              if (settings["Query"].count("Keywords") > 0 && settings["Query"]["Keywords"].is_array()) {
                  url += "track=";
                  for (auto& kw : settings["Query"]["Keywords"]) {
                      url += kw.get<std::string>() + ",";
                  }
              }
              return url;
          }) |
          debounce(milliseconds(1000), mainthread) |
          distinct_until_changed() |
          tap([](string url){
              cerr << "url = " << url.c_str() << endl;
          }) |
          replay(1) |
          ref_count() |
          as_dynamic();

    // ==== Tweets

    observable<string> chunks;

    // switch to new connection whenever the url changes
    chunks = urlchanges |
         rxo::map([&](const string& url){
             // ==== Constants - flags
             const bool isFilter = url.find("/statuses/filter") != string::npos;
             string method = isFilter ? "POST" : "GET";

             /*  Create new http request for a stream of tweets

                 If an error is encountered, function on_error_resume_next ensures the error
                 item is replaced with some correct observable value.

                 In this case return never, which never completes and never emits a value.
                 The never will be cancelled when the url changes
             */
             return twitterrequest(tweetthread, factory, url, method, settings["ConsumerKey"], settings["ConsumerSecret"], settings["AccessTokenKey"], settings["AccessTokenSecret"]) |
                    // handle invalid requests by waiting for a trigger to try again
                    on_error_resume_next([](std::exception_ptr ep){
                        cerr << rxu::what(ep) << endl;
                        return rxs::never<string>();
                    });
         }) |
         switch_on_next();


    // parse tweets
    auto tweets = chunks |
          parsetweets(poolthread, tweetthread) |
          rxo::map([](parsedtweets p){
              p.errors |
              tap([](parseerror e){
                  cerr << rxu::what(e.ep) << endl;
              }) |
              subscribe<parseerror>();
              return p.tweets;
          }) |
          merge(tweetthread);

    auto ts = tweets |
              retry() |
              publish() |
              ref_count() |
              as_dynamic();

    std::vector<observable<Reducer>> reducers;

}
