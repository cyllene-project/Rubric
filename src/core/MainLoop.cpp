// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <core/MainLoop.h>
#include <unistd.h>


using namespace rubric::core;

rxcpp::schedulers::scheduler MainLoop::getScheduler() const {
    return  runLoop.get_scheduler();
}

MainLoop::MainThread MainLoop::getMainThread() const {
    return rxcpp::observe_on_run_loop(runLoop);
}

MainLoop::FrameBus MainLoop::getFrameBus() const {
    return framebus.get_observable();
}

bool MainLoop::empty() const {
    return runLoop.empty();
}

void MainLoop::run() {

    auto frameout = framebus.get_subscriber();

    auto sendframe = [=]() {
        frameout.on_next(1);
    };


    while (lifetime.is_subscribed()) {


        // Poll for OS events like ctrl+C

        if (!lifetime.is_subscribed()) {
            break;
        }

        while (!runLoop.empty() && runLoop.peek().when < runLoop.now()) {
            runLoop.dispatch();
        }

        sendframe();

        while (!runLoop.empty() && runLoop.peek().when < runLoop.now()) {
            runLoop.dispatch();
        }


        // render

    }

}

std::thread::id MainLoop::getMainThreadId() const {
    std::this_thread::get_id();
}

MainLoop::EventLoop MainLoop::getEventLoop() const {
    return rxcpp::observe_on_event_loop();
}


