// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_CONTEXT_H
#define RUBRIC_CONTEXT_H

#include <DisplayManager.h>
#include <memory>

namespace rubric::core {

    using namespace rubric::draw;

    class Context {

    public:
        Context();
        static std::shared_ptr<Context> getInstance();
    private:
        static std::shared_ptr<Context> instance;
        std::unique_ptr<DisplayManager> displayManager;

    };
}

#endif //RUBRIC_CONTEXT_H
