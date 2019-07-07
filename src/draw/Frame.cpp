// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "draw/Frame.h"

using namespace rubric::draw;

Frame::Frame() {
    pushState();
}

Frame::State & Frame::getCurrentState() {
    return states.front();
}

void Frame::pushState() {
    states.emplace_back();
}