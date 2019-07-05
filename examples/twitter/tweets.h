#ifndef TWEETS_H
#define TWEETS_H

#include <rxcpp/rx.hpp>
#include "rxcurl.h"
#include "model.h"
#include <oauth.h>
#include <fstream>

namespace tweets {


inline milliseconds timestamp_ms(const model::Tweet& tw) {
    auto& tweet = tw.data->tweet;
    auto t = milliseconds(stoll(tweet["timestamp_ms"].get<string>()));
    return t;
}

auto isEndOfTweet = [](const string& s){
    if (s.size() < 2) return false;
    auto it0 = s.begin() + (s.size() - 2);
    auto it1 = s.begin() + (s.size() - 1);
    return *it0 == '\r' && *it1 == '\n';
};

struct parseerror {
    std::exception_ptr ep;
};
struct parsedtweets {
    observable<model::Tweet> tweets;
    observable<parseerror> errors;  
};

auto parsetweets(observe_on_one_worker worker, observe_on_one_worker tweetthread) -> function<observable<parsedtweets>(observable<string>)>;

inline auto onlytweets() -> function<observable<model::Tweet>(observable<model::Tweet>)> {
    return [](observable<model::Tweet> s){
        return s | filter([](const model::Tweet& tw){
            auto& tweet = tw.data->tweet;
            return !!tweet.count("timestamp_ms");
        });
    };
}

enum class errorcodeclass {
    Invalid,
    TcpRetry,
    ErrorRetry,
    StatusRetry,
    RateLimited
};

inline errorcodeclass errorclassfrom(const rxcurl::http_exception& ex) {
    switch(ex.code()) {
        case CURLE_COULDNT_RESOLVE_HOST:
        case CURLE_COULDNT_CONNECT:
        case CURLE_OPERATION_TIMEDOUT:
        case CURLE_BAD_CONTENT_ENCODING:
        case CURLE_REMOTE_FILE_NOT_FOUND:
            return errorcodeclass::ErrorRetry;
        case CURLE_GOT_NOTHING:
        case CURLE_PARTIAL_FILE:
        case CURLE_SEND_ERROR:
        case CURLE_RECV_ERROR:
            return errorcodeclass::TcpRetry;
        default:
            if (ex.code() == CURLE_HTTP_RETURNED_ERROR || ex.httpStatus() > 200) {
                if (ex.httpStatus() == 420) {
                    return errorcodeclass::RateLimited;
                } else if (ex.httpStatus() == 404 ||
                    ex.httpStatus() == 406 ||
                    ex.httpStatus() == 413 ||
                    ex.httpStatus() == 416) {
                    return errorcodeclass::Invalid;
                }
            }
    };
    return errorcodeclass::StatusRetry;
}

auto filechunks = [](observe_on_one_worker tweetthread, string filepath) {
    return observable<>::create<string>([=](subscriber<string> out){

        auto values = make_tuple(ifstream{filepath}, string{});
        auto state = make_shared<decltype(values)>(move(values));

        // creates a worker whose lifetime is the same as this subscription
        auto coordinator = tweetthread.create_coordinator(out.get_subscription());

        auto controller = coordinator.get_worker();

        auto producer = [out, state](const rxsc::schedulable& self) {

            if (!out.is_subscribed()) {
                // terminate loop
                return;
            }

            if (getline(get<0>(*state), get<1>(*state)))
            {
                get<1>(*state)+="\r\n";
                out.on_next(get<1>(*state));
            } else {
                out.on_completed();
                return;
            }

            // tail recurse this same action to continue loop
            self();
        };

        controller.schedule(coordinator.act(producer));
    });
};

auto twitter_stream_reconnection = [](observe_on_one_worker tweetthread){
    return [=](observable<string> chunks){
        return chunks |
            // https://dev.twitter.com/streaming/overview/connecting
            timeout(seconds(90), tweetthread) |
            on_error_resume_next([=](std::exception_ptr ep) -> observable<string> {
                try {rethrow_exception(ep);}
                catch (const rxcurl::http_exception& ex) {
                    cerr << ex.what() << endl;
                    switch(errorclassfrom(ex)) {
                        case errorcodeclass::TcpRetry:
                            cerr << "reconnecting after TCP error" << endl;
                            return observable<>::empty<string>();
                        case errorcodeclass::ErrorRetry:
                            cerr << "error code (" << ex.code() << ") - ";
                        case errorcodeclass::StatusRetry:
                            cerr << "http status (" << ex.httpStatus() << ") - waiting to retry.." << endl;
                            return observable<>::timer(seconds(5), tweetthread) | model::stringandignore();
                        case errorcodeclass::RateLimited:
                            cerr << "rate limited - waiting to retry.." << endl;
                            return observable<>::timer(minutes(1), tweetthread) | model::stringandignore();
                        case errorcodeclass::Invalid:
                            cerr << "invalid request - propagate" << endl;
                        default:
                            cerr << "unrecognized error - propagate" << endl;
                    };
                }
                catch (const timeout_error& ex) {
                    cerr << "reconnecting after timeout" << endl;
                    return observable<>::empty<string>();
                }
                catch (const exception& ex) {
                    cerr << "unknown exception - terminate" << endl;
                    cerr << ex.what() << endl;
                    terminate();
                }
                catch (...) {
                    cerr << "unknown exception - not derived from std::exception - terminate" << endl;
                    terminate();
                }
                return observable<>::error<string>(ep, tweetthread);
            }) |
            repeat();
    };
};

auto twitterrequest = [](observe_on_one_worker tweetthread, ::rxcurl::rxcurl factory, string URL, string method, string CONS_KEY, string CONS_SEC, string ATOK_KEY, string ATOK_SEC){

    return observable<>::defer([=](){

        string url;
        {
            char* signedurl = nullptr;
            RXCPP_UNWIND_AUTO([&](){
                if (!!signedurl) {
                    free(signedurl);
                }
            });
            signedurl = oauth_sign_url2(
                URL.c_str(), NULL, OA_HMAC, method.c_str(),
                CONS_KEY.c_str(), CONS_SEC.c_str(), ATOK_KEY.c_str(), ATOK_SEC.c_str()
            );
            url = signedurl;
        }

        cerr << "start twitter stream request" << endl;

        return factory.create(rxcurl::http_request{url, method, {}, {}}) |
            rxo::map([](rxcurl::http_response r){
                return r.body.chunks;
            }) |
            finally([](){cerr << "end twitter stream request" << endl;}) |
            merge(tweetthread);
    }) |
    twitter_stream_reconnection(tweetthread);
};

auto sentimentrequest = [](observe_on_one_worker worker, ::rxcurl::rxcurl factory, string url, string key, vector<string> text) -> observable<string> {

    std::map<string, string> headers;
    headers["Content-Type"] = "application/json";
    headers["Authorization"] = "Bearer " + key;

    auto body = json::parse(R"({"Inputs":{"input1":[]},"GlobalParameters":{}})");

    static const regex nonascii(R"([^A-Za-z0-9])");

    auto& input1 = body["Inputs"]["input1"];
    for(auto& t : text) {

        auto ascii = regex_replace(t, nonascii, " ");

        input1.push_back({{"tweet_text", ascii}});
    }

    return observable<>::defer([=]() -> observable<string> {
        return factory.create(rxcurl::http_request{url, "POST", headers, body.dump()}) |
            rxo::map([](rxcurl::http_response r){
                return r.body.complete;
            }) |
            merge(worker) |
            tap([=](exception_ptr){
                cout << body << endl;
            });
    });
};

auto perspectiverequest = [](observe_on_one_worker worker, ::rxcurl::rxcurl factory, string url, string key, string text) -> observable<string> {

    std::map<string, string> headers;
    headers["Content-Type"] = "application/json";

    url += "?key=" + key;

    auto body = json::parse(R"({"comment": {"text": ""}, "languages": ["en"], "requestedAttributes": {"TOXICITY":{}, "INFLAMMATORY":{}, "SPAM":{}}, "doNotStore": true })");

    body["comment"]["text"] = text;

    return observable<>::defer([=]() -> observable<string> {

        return factory.create(rxcurl::http_request{url, "POST", headers, body.dump()}) |
            rxo::map([](rxcurl::http_response r){
                return r.body.complete;
            }) |
            merge(worker) |
            tap([=](exception_ptr){
                cout << body << endl;
            });
    });
};
    

}

#endif