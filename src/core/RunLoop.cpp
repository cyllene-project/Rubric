// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <core/RunLoop.h>
#include <unistd.h>


using namespace rubric::core;

rxcpp::schedulers::scheduler RunLoop::get_scheduler() const {
    return  runLoop.get_scheduler();
}

auto RunLoop::observe_on_run_loop() const {
    return rxcpp::observe_on_run_loop(runLoop);
}

bool RunLoop::empty() const {
    return runLoop.empty();
}

void RunLoop::run() {
    running = true;

    while (true) {

        if (runLoop.empty())
            usleep(1000);

        while (!runLoop.empty() && runLoop.peek().when < runLoop.now()) {
            runLoop.dispatch();
        }

        if(!running)
            break;
    }

}

RunLoop::RunLoop() {

}
