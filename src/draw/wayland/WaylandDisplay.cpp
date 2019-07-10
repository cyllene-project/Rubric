// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "WaylandDisplay.h"
#include "WaylandSurface.h"
#include <wayland-client.h>
#include <iostream>

using namespace rubric::draw::wayland;
using namespace rubric::draw;

static void log_handler (const char *format, va_list args) {
    std::cout << printf(format, args) << std::endl;
}

WaylandDisplay::WaylandDisplay(Context & ctxt, WLDisplay display) noexcept:
Display(ctxt),
wlDisplay(display){
    wlRegistry = wl_display_get_registry(display);
    wl_log_set_handler_client(log_handler);
}

std::unique_ptr<Surface> WaylandDisplay::createSurface(SurfaceType, std::shared_ptr<Surface>, int, int, int, int) {
    return std::make_unique<WaylandSurface>(*this);
}

