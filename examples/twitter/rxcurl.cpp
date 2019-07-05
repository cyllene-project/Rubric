// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "rxcurl.h"

rxcurl::rxcurl rxcurl::create_rxcurl() {
    rxcurl r{std::make_shared<rxcurl_state>()};
    return r;
};

size_t rxcurl::rxcurlhttpCallback(char* ptr, size_t size, size_t nmemb, subscriber<std::string>* out) {
    int iRealSize = size * nmemb;

    string chunk;
    chunk.assign(ptr, iRealSize);
    out->on_next(chunk);

    return iRealSize;
}