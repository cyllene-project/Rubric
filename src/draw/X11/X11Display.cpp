// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "X11Display.h"
#include "X11Surface.h"

using namespace rubric::draw::x11;
using namespace rubric::draw;

X11Display::X11Display(::Display *xdisplay):
xDisplay(xdisplay)
{}

std::shared_ptr<Surface> X11Display::createSurface() const {
    return std::make_shared<X11Surface>();
}