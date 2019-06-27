// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "X11Display.h"
#include "X11Surface.h"

using namespace rubric::draw::x11;
using namespace rubric::draw;

X11Display::X11Display(x::connection & xconnection) :
connection(xconnection) {}

std::unique_ptr<Surface> X11Display::createSurface(SurfaceType, int, int, int, int) {
    return std::make_unique<X11Surface>(*this);
}