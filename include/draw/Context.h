// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_CONTEXT_H
#define RUBRIC_CONTEXT_H

#include <draw/DisplayManager.h>
#include <memory>
#include "core/RunLoop.h"

namespace rubric::core {

    using namespace rubric::draw;

    class Context {

    public:
        Context() noexcept;
        static Context & getInstance();
        const DisplayManager & getDisplayManager() const;
    private:
        static Context instance;
        DisplayManager displayManager;
        RunLoop runLoop;

    };
}

#endif //RUBRIC_CONTEXT_H
