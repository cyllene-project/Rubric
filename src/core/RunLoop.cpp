// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "core/RunLoop.h"

using namespace rubric::core;

rxcpp::schedulers::scheduler RunLoop::get_scheduler() const {
    return  rxcpp_run_loop.get_scheduler();
}

auto RunLoop::observe_on_run_loop() const {
    return rxcpp::observe_on_run_loop(rxcpp_run_loop);
}

bool RunLoop::empty() const {
    return rxcpp_run_loop.empty();
}

void RunLoop::run() const {

    while (!rxcpp_run_loop.empty()) {
        while (!rxcpp_run_loop.empty() && rxcpp_run_loop.peek().when < rxcpp_run_loop.now()) {
            rxcpp_run_loop.dispatch();
        }
    }

}