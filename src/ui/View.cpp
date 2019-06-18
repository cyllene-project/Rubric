// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "View.h"

using namespace rubric::ui;

bool View::isVisible() const {
    return visible;
}

void View::setVisible(bool visible) {
    this->visible = visible;
}

std::weak_ptr<View> View::getParent() const {
    return parent;
}