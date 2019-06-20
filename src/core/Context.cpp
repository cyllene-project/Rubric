// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <iostream>
#include "Context.h"

using namespace rubric::core;

namespace rubric::core {

    Context Context::instance;

    Context & Context::getInstance() {
        return instance;
    }

    Context::Context() noexcept {
        auto display = displayManager.getDefaultDisplay();
        if (display == nullptr) {
            display = displayManager.openDisplay(std::string());
        }
    }

    const DisplayManager& Context::getDisplayManager() const {
        return displayManager;
    }
}