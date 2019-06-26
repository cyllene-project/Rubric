// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_RENDERER_H
#define RUBRIC_RENDERER_H

#include <memory>
#include "draw/Surface.h"

namespace rubric::draw {

    class Renderer {

    public:
        virtual void realize(std::shared_ptr<Surface> &) = 0;


    };
}

#endif //RUBRIC_RENDERER_H
