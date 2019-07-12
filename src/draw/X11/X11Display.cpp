// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "X11Display.h"
#include "X11Surface.h"

#include <draw/Context.h>
#include <core/MainLoop.h>

using namespace rubric::draw::x11;
using namespace rubric::draw;
using namespace std::chrono;
using duration_type = rxcpp::schedulers::scheduler::clock_type::time_point::duration;

X11Display::X11Display(rubric::draw::Context & ctxt) :
    Display(ctxt),
    eventHandler(connection) {

    auto values = rxcpp::observable<>::interval(milliseconds(50 / 3))
            .time_interval();

    values.subscribe(
            [&](duration_type v) {
                connection.flush();
                eventHandler.handle(connection.wait_for_event());
            },
            [](std::exception_ptr ep) {
                try {
                    std::rethrow_exception(ep);
                } catch (const std::exception &ex) {
                    printf("OnError: %s\n", ex.what());
                }
            });
}

std::shared_ptr<Surface> X11Display::createSurface(SurfaceType surfaceType,
                                                   std::shared_ptr<Surface> parent,
                                                   int x, int y,
                                                   int width, int height) {

    auto surface = std::make_shared<X11Surface>(*this, connection, surfaceType, x, y, width, height);

    return surface;
}