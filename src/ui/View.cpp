// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <Rubric.h>
#include "ui/View.h"


namespace rubric::ui {

    RUBRIC_REGISTER_ABSTRACT_TYPE(View, Object)

    bool View::isVisible() const {
        return visible;
    }

    void View::setVisible(bool _visible) {
        visible = _visible;
    }

    std::weak_ptr<View> View::getParent() const {
        return parent;
    }
}