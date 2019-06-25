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

WaylandDisplay::WaylandDisplay(WLDisplay display):
wlDisplay(display){
    wlRegistry = wl_display_get_registry(display);
    wl_log_set_handler_client(log_handler);
}

std::unique_ptr<Surface> WaylandDisplay::createSurface() {
    return std::make_unique<WaylandSurface>(*this);
}

