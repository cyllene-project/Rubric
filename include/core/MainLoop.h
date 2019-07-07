// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_RUNLOOP_H
#define RUBRIC_RUNLOOP_H

#include <rxcpp/rx.hpp>

namespace rubric::core {

    class MainLoop {

    public:

        using MainThread = rxcpp::observe_on_one_worker;
        using EventLoop = MainThread;
        using FrameBus = rxcpp::observable<int>;

        rxcpp::schedulers::scheduler getScheduler() const;

        MainThread getMainThread() const;

        EventLoop getEventLoop() const;

        std::thread::id getMainThreadId() const;

        FrameBus getFrameBus() const;

        bool empty() const;

        void run();

    private:

        rxcpp::schedulers::run_loop runLoop;

        rxcpp::composite_subscription lifetime;

        EventLoop eventLoop = rxcpp::observe_on_event_loop();

        rxcpp::subjects::subject<int> framebus;

    };

}
#endif //RUBRIC_RUNLOOP_H
