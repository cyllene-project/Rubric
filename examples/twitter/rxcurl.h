#ifndef RXCURL_H
#define RXCURL_H

#include <rxcpp/rx.hpp>
#include <curl/curl.h>
#include <cstring>

namespace rxcurl {

using namespace rxcpp;
using namespace rxcpp::rxo;
using namespace rxcpp::rxs;
using namespace std;
using namespace std::chrono;

struct rxcurl_state
{
    ~rxcurl_state(){
        if (!!curlm) {
            worker.as_blocking().subscribe();
            curl_multi_cleanup(curlm);
            curlm = nullptr;
        }
    }
    rxcurl_state() : thread(observe_on_new_thread()), worker(), curlm(curl_multi_init()) {
        worker = observable<>::create<CURLMsg*>([this](subscriber<CURLMsg*> out){
                while(out.is_subscribed()) {
                    int running = 0;
                    curl_multi_perform(curlm, &running);
                    for(;;) {
                        CURLMsg *message = nullptr;
                        int remaining = 0;
                        message = curl_multi_info_read(curlm, &remaining);
                        out.on_next(message);
                        if (!!message && remaining > 0) {
                            continue;
                        }
                        break;
                    }
                    int handlecount = 0;
                    curl_multi_wait(curlm, nullptr, 0, 500, &handlecount);
                    if (handlecount == 0) {
                        this_thread::sleep_for(milliseconds(100));
                    }
                }
                out.on_completed();
            }) |
            subscribe_on(thread) |
            finally([](){cerr << "rxcurl worker exit" << endl;}) |
            publish() |
            connect_forever();
    }
    rxcurl_state(const rxcurl_state&) = delete;
    rxcurl_state& operator=(const rxcurl_state&) = delete;
    rxcurl_state(rxcurl_state&&) = delete;
    rxcurl_state& operator=(rxcurl_state&&) = delete;

    observe_on_one_worker thread;
    observable<CURLMsg*> worker;
    CURLM* curlm;
};
struct http_request
{
    string url;
    string method;
    std::map<string, string> headers;
    string body;
};
struct http_state
{
    ~http_state() {
        if (!!curl) {
            // remove on worker thread
            auto localcurl = curl;
            auto localheaders = headers;
            auto localrxcurl = rxcurl;
            auto localRequest = request;
            chunkbus.get_subscription().unsubscribe();
            subscriber<string>* localChunkout = chunkout.release();
            rxcurl->worker
                .take(1)
                .tap([=](CURLMsg*){
                    //cerr << "rxcurl request destroy: " << localRequest.method << " - " << localRequest.url << endl;
                    curl_multi_remove_handle(localrxcurl->curlm, localcurl);
                    curl_easy_cleanup(localcurl);
                    curl_slist_free_all(localheaders);
                    delete localChunkout;
                })
                .subscribe();

            curl = nullptr;
            headers = nullptr;
        }
    }
    explicit http_state(shared_ptr<rxcurl_state> m, http_request r) : rxcurl(m), request(r), code(CURLE_OK), httpStatus(0), curl(nullptr), headers(nullptr) {
        error.resize(CURL_ERROR_SIZE);
    }
    http_state(const http_state&) = delete;
    http_state& operator=(const http_state&) = delete;
    http_state(http_state&&) = delete;
    http_state& operator=(http_state&&) = delete;

    shared_ptr<rxcurl_state> rxcurl;
    http_request request;
    string error;
    CURLcode code;
    int httpStatus;
    subjects::subject<string> chunkbus;
    unique_ptr<subscriber<string>> chunkout;
    CURL* curl;
    struct curl_slist *headers;
    vector<string> strings;
};
struct http_exception : runtime_error
{
    explicit http_exception(const shared_ptr<http_state>& s) : runtime_error(s->error), state(s) {
    }

    CURLcode code() const {
        return state->code;
    }
    int httpStatus() const {
        return state->httpStatus;
    }

    shared_ptr<http_state> state;
};
struct http_body
{
    observable<string> chunks;
    observable<string> complete;
};
struct http_response
{
    const http_request request;
    http_body body;

    CURLcode code() const {
        return state->code;
    }
    int httpStatus() const {
        return state->httpStatus;
    }

    shared_ptr<http_state> state;
};

size_t rxcurlhttpCallback(char* ptr, size_t size, size_t nmemb, subscriber<string>* out);

struct rxcurl
{
    shared_ptr<rxcurl_state> state;

    observable<http_response> create(http_request request) const {
        return observable<>::create<http_response>([=](subscriber<http_response>& out){

            auto requestState = make_shared<http_state>(state, request);

            http_response r{request, http_body{}, requestState};

            r.body.chunks = r.state->chunkbus.get_observable()
                .tap([requestState](const string&){}); // keep connection alive

            r.body.complete = r.state->chunkbus.get_observable()
                .tap([requestState](const string&){}) // keep connection alive
                .start_with(string{})
                .sum() 
                .replay(1)
                .ref_count();
            
            // subscriber must subscribe during the on_next call to receive all marbles
            out.on_next(r);
            out.on_completed();

            auto localState = state;

            // start on worker thread
            state->worker
                .take(1)
                .tap([r, localState](CURLMsg*){

                    auto curl = curl_easy_init();

                    auto& request = r.state->request;

                    //cerr << "rxcurl request: " << request.method << " - " << request.url << endl;

                    // ==== cURL Setting
                    curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());

                    if (request.method == "POST") {
                        // - POST data
                        curl_easy_setopt(curl, CURLOPT_POST, 1L);
                        // - specify the POST data
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());
                    }

                    auto& strings = r.state->strings;
                    auto& headers = r.state->headers;
                    for (auto& h : request.headers) {
                        strings.push_back(h.first + ": " + h.second);
                        headers = curl_slist_append(headers, strings.back().c_str());
                    }

                    if (!!headers) {
                        /* set our custom set of headers */ 
                        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                    }
    
                    // - User agent name
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, "rxcpp curl client 1.1");
                    // - HTTP STATUS >=400 ---> ERROR
                    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

                    // - Callback function
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rxcurlhttpCallback);
                    // - Write data
                    r.state->chunkout.reset(new subscriber<string>(r.state->chunkbus.get_subscriber()));
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)r.state->chunkout.get());

                    // - keep error messages
                    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &r.state->error[0]); 

                    r.state->curl = curl;
                    curl_multi_add_handle(localState->curlm, curl);
                })
                .subscribe();

            weak_ptr<http_state> wrs = requestState;

            // extract completion and result
            state->worker
                .filter([wrs](CURLMsg* message){
                    auto rs = wrs.lock();
                    return !!rs && !!message && message->easy_handle == rs->curl && message->msg == CURLMSG_DONE;
                })
                .take(1)
                .tap([wrs](CURLMsg* message){
                    auto rs = wrs.lock();
                    if (!rs) {
                        return;
                    }

                    rs->error.resize(strlen(&rs->error[0]));

                    auto chunkout = rs->chunkbus.get_subscriber();

                    long httpStatus = 0;

                    curl_easy_getinfo(rs->curl, CURLINFO_RESPONSE_CODE, &httpStatus);
                    rs->httpStatus = httpStatus;

                    if(message->data.result != CURLE_OK) {
                        rs->code = message->data.result;
                        if (rs->error.empty()) {
                            rs->error = curl_easy_strerror(message->data.result);
                        }
                        //cerr << "rxcurl request fail: " << httpStatus << " - " << rs->error << endl;
                        observable<>::error<string>(http_exception(rs)).subscribe(chunkout);
                        return;
                    } else if (httpStatus > 499) {
                        //cerr << "rxcurl request http fail: " << httpStatus << " - " << rs->error << endl;
                        observable<>::error<string>(http_exception(rs)).subscribe(chunkout);
                        return;
                    }

                    //cerr << "rxcurl request complete: " << httpStatus << " - " << rs->error << endl;
                    chunkout.on_completed();
                })
                .subscribe();
        });
    }
};

rxcurl create_rxcurl();

}

#endif