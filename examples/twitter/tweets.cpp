// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)
#include <rxcpp/rx.hpp>
#include "tweets.h"
using namespace rxcpp;
using namespace rxcpp::rxo;
using namespace rxcpp::rxs;
using namespace std;
using namespace tweets;

auto tweets::parsetweets(observe_on_one_worker worker, observe_on_one_worker tweetthread) -> function<observable<parsedtweets>(observable<string>)> {
    return [=](observable<string> chunks) -> observable<parsedtweets> {
        return rxs::create<parsedtweets>([=](subscriber<parsedtweets> out){
            // create strings split on \r
            auto strings = chunks |
                           concat_map([](const string& s){
                               auto splits = ::util::split(s, "\r\n");
                               return iterate(move(splits));
                           }) |
                           filter([](const string& s){
                               return !s.empty();
                           }) |
                           publish() |
                           ref_count();

            // filter to last string in each line
            auto closes = strings |
                          filter(isEndOfTweet) |
                          rxo::map([](const string&){return 0;});

            // group strings by line
            auto linewindows = strings |
                               window_toggle(closes | start_with(0), [=](int){return closes;});

            // reduce the strings for a line into one string
            auto lines = linewindows |
                         flat_map([](const observable<string>& w) {
                             return w | start_with<string>("") | sum();
                         });

            int count = 0;
            rxsub::subject<parseerror> errorconduit;
            observable<model::Tweet> tweets = lines |
                                              filter([](const string& s){
                                                  return s.size() > 2 && s.find_first_not_of("\r\n") != string::npos;
                                              }) |
                                              group_by([count](const string&) mutable -> int {
                                                  return ++count % std::thread::hardware_concurrency();}) |
                                              rxo::map([=](observable<string> shard) {
                                                  return shard |
                                                         observe_on(worker) |
                                                         rxo::map([=](const string& line) -> observable<model::Tweet> {
                                                             try {
                                                                 auto tweet = json::parse(line);
                                                                 return rxs::from(model::Tweet(tweet));
                                                             } catch (...) {
                                                                 errorconduit.get_subscriber().on_next(parseerror{std::current_exception()});
                                                             }
                                                             return rxs::empty<model::Tweet>();
                                                         }) |
                                                         merge() |
                                                         as_dynamic();
                                              }) |
                                              merge(tweetthread) |
                                              tap([](const model::Tweet&){},[=](){
                                                  errorconduit.get_subscriber().on_completed();
                                              }) |
                                              finally([=](){
                                                  errorconduit.get_subscriber().unsubscribe();
                                              });

            out.on_next(parsedtweets{tweets, errorconduit.get_observable()});
            out.on_completed();

            return out.get_subscription();
        });
    };
}
