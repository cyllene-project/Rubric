// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "X11Backend.h"
#include "X11Display.h"

using namespace rubric::draw::x11;
using namespace rubric::draw;
using namespace rubric;

RUBRIC_DRAW_REGISTER_BACKEND(X11Backend)

std::unique_ptr<Display> X11Backend::open(rubric::draw::Context &, const std::string &displayName) const {

    static x::connection connection;
    static x::registry registry(connection);

    auto display = std::make_unique<X11Display>(connection);

    return display;
}

std::string X11Backend::getName() const {
    return "x11";
}