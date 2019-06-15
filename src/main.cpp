// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <memory>
#include "Rubric.h"
#include "Display.h"

#include "core/Context.h"

namespace rubric {

    static std::shared_ptr<core::Context> context = nullptr;

    static bool initialized = false;

    bool init() {

        if (initialized)
            return true;

        auto display = draw::Display::open_default();

        context = std::make_shared<core::Context>();

        initialized = true;


        return initialized;
    }
}