// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "WaylandSurface.h"

using namespace rubric;
using namespace rubric::draw;
using namespace rubric::draw::wayland;

WaylandSurface::WaylandSurface(rubric::draw::wayland::WaylandDisplay & display):
wDisplay(display){ }

Display & WaylandSurface::getDisplay() {
    return wDisplay;
}