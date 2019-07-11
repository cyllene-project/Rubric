// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "draw/Surface.h"

using namespace rubric::draw;
using namespace rubric;


void Surface::show() {

}

void Surface::hide() {

}

void Surface::resize(int width, int height) {

}

void Surface::tick() const {
    render.get_subscriber().on_next(Region { });
}


rxcpp::observable<Region> Surface::renderUpdates() {
    return render.get_observable();
}

rxcpp::observable<Event> Surface::eventStream() {
    return events.get_observable();
}

rxcpp::observable<std::shared_ptr<Surface>> Surface::sizeUpdates() {
    return sizeChanged.get_observable();
}