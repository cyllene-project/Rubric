// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_DISPLAY_H
#define RUBRIC_DISPLAY_H

#include <string>
#include <memory>
#include <draw/Surface.h>

namespace rubric::draw {

    class Context;

    class Display {

    public:
        explicit Display(Context &) noexcept;
        virtual std::shared_ptr<Surface> createSurface(SurfaceType, std::shared_ptr<Surface>, int, int, int, int) = 0;

        Context & getContext() const noexcept;

    protected:
        Context & context;

    };

}

#endif //RUBRIC_DISPLAY_H
