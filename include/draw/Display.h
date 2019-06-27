// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_DISPLAY_H
#define RUBRIC_DISPLAY_H

#include <string>
#include <memory>
#include <draw/Surface.h>

namespace rubric::draw {

    class Display {

    public:
        virtual std::unique_ptr<Surface> createSurface(SurfaceType, int, int, int, int) = 0;
    };

}

#endif //RUBRIC_DISPLAY_H
