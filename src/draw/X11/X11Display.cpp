// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "X11Display.h"

using namespace rubric::draw::x11;

X11Display::X11Display(::Display *xdisplay):
xDisplay(xdisplay)
{}