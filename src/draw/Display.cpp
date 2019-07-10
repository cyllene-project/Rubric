// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "draw/Display.h"

using namespace rubric::draw;

Display::Display(rubric::draw::Context & ctxt) noexcept :
context(ctxt){ }

Context& Display::getContext() const noexcept {
    return context;
}