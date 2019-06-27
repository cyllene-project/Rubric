// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <iostream>
#include "draw/Context.h"

using namespace rubric::core;

namespace rubric::draw {

    Context Context::instance;

    Context & Context::getInstance() {
        return instance;
    }

    Context::Context() noexcept {

        auto display = displayManager.getDefaultDisplay();

        if (display == nullptr) {
            displayManager.openDisplay("");
        }

    }

    const DisplayManager& Context::getDisplayManager() const {
        return displayManager;
    }
}