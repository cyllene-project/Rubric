// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <core/Event.h>
#include "X11Backend.h"
#include "X11Display.h"
#include "X11EventHandler.h"
#include <X11/cursorfont.h> // XC_cross

using namespace rubric::draw::x11;
using namespace rubric::draw;
using namespace rubric;

RUBRIC_DRAW_REGISTER_BACKEND(X11Backend)

std::unique_ptr<rubric::draw::Display>
X11Backend::open(rubric::draw::Context &ctxt, const std::string &displayName) const {
    static x::connection connection;
    static x::registry registry(connection);
    static EventHandler<x::connection &> eventHandler(connection);
    registry.attach(0, &eventHandler);


    using duration_type = rxcpp::schedulers::scheduler::clock_type::time_point::duration;
    using namespace std::chrono;

    auto values = rxcpp::observable<>::interval(milliseconds(50 / 3))
            .time_interval();

    values.subscribe(
        [&](duration_type v) {
            connection.flush();
            registry.dispatch(connection.wait_for_event());
        },
        [](std::exception_ptr ep) {
            try {
                std::rethrow_exception(ep);
            } catch (const std::exception &ex) {
                printf("OnError: %s\n", ex.what());
            }
        });


    return std::make_unique<X11Display>(ctxt, connection);
}

std::string X11Backend::getName() const {
    return "x11";
}