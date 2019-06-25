// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "X11Surface.h"

using namespace rubric;
using namespace rubric::draw;
using namespace rubric::draw::x11;

X11Surface::X11Surface(X11Display & display):
xDisplay(display){

}

rubric::draw::Display & X11Surface::getDisplay() {
    return xDisplay;
}