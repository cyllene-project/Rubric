// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_CONTEXT_H
#define RUBRIC_CONTEXT_H

#include <draw/DisplayManager.h>
#include <memory>
#include "core/MainLoop.h"

namespace rubric::draw {

    using namespace rubric::core;

    class Context {

    public:
        explicit Context() noexcept;
        static Context & getInstance();
        std::shared_ptr<Display> getDefaultDisplay() const;
        const DisplayManager & getDisplayManager() const;
        MainLoop & getRunLoop();
    private:
        static Context instance;
        DisplayManager displayManager;
        MainLoop runLoop;

    };
}

#endif //RUBRIC_CONTEXT_H
