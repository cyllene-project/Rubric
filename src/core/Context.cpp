// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <iostream>
#include "Context.h"

using namespace rubric::core;

namespace rubric::core {

    std::shared_ptr<Context> Context::instance = std::make_shared<core::Context>();

    std::shared_ptr<Context> Context::getInstance() {
        return instance;
    }

    Context::Context() {
        displayManager = std::make_unique<DisplayManager>();
        auto display = displayManager->getDefaultDisplay();
        if (display == nullptr) {
            display = displayManager->openDisplay(std::string());
        }

    }
}