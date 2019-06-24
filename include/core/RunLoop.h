// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_RUNLOOP_H
#define RUBRIC_RUNLOOP_H

#include <rxcpp/rx.hpp>

namespace rubric::core {

    class RunLoop {

    public:

        rxcpp::schedulers::scheduler get_scheduler() const;

        auto observe_on_run_loop() const;

        bool empty() const;

        void run() const;

    private:

        rxcpp::schedulers::run_loop rxcpp_run_loop;

    };

}
#endif //RUBRIC_RUNLOOP_H
