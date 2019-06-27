// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <iostream>
#include "draw/DisplayManager.h"
#include "Backend.h"

namespace rubric::draw {

    void DisplayManager::openDisplay(std::string name) {

        auto backend = std::getenv("RUBRIC_BACKEND");

        if (backend == nullptr) {
            throw std::invalid_argument("RUBRIC_BACKEND environment variable not set");
        }

        auto backends = Backend::getBackends();

        if (backends.count(backend) > 0) {
            auto be = backends[backend]->create();
            setDefaultDisplay(be->open(name));
        }


    }

    std::shared_ptr<Display> DisplayManager::getDefaultDisplay() const {
        return defaultDisplay;
    }

    void DisplayManager::setDefaultDisplay(std::shared_ptr<Display> display) {
        defaultDisplay = std::move(display);
    }
}