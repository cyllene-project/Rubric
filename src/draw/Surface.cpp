// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "draw/Surface.h"

using namespace rubric::draw;

/*
rxcpp::observable<Surface&> Surface::onRender() {
    return render.get_observable();
}*/

void Surface::show() {

}

void Surface::hide() {

}

void Surface::resize(int width, int height) {

}

void Surface::tick() const {
    render.get_subscriber().on_next(Region { });
}