// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_CONTEXT_H
#define RUBRIC_CONTEXT_H

#include <memory>
#include "core/MainLoop.h"
#include <draw/Display.h>

namespace rubric::draw {

    class Context {

    public:
        explicit Context() noexcept;

        static Context & getInstance();

        std::shared_ptr<Display> getDefaultDisplay() const;

        core::MainLoop & getRunLoop();

    private:

        class ContextImpl;

        std::unique_ptr<ContextImpl> impl;

        static Context instance;

    };
}

#endif //RUBRIC_CONTEXT_H
