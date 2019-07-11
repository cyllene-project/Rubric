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

    return std::make_unique<X11Display>(ctxt);
}

std::string X11Backend::getName() const {
    return "x11";
}