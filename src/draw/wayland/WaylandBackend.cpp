// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "WaylandBackend.h"
#include "WaylandDisplay.h"

using namespace rubric::draw::wayland;
using namespace rubric::draw;

RUBRIC_DRAW_REGISTER_BACKEND(WaylandBackend)

std::unique_ptr<Display> WaylandBackend::open(rubric::draw::Context &, const std::string &displayName) const {

    WLDisplay wl_display;

    if (std::getenv("XDG_RUNTIME_DIR") == nullptr)
        throw std::runtime_error("XDG_RUNTIME_DIR not set");

    wl_display = wl_display_connect(displayName.c_str());
    if (!wl_display)
        throw std::runtime_error("Unable to connect to the Wayland Server");

    auto display = std::make_unique<WaylandDisplay>(wl_display);

    return display;

}

std::string WaylandBackend::getName() const {
    return "wayland";
}