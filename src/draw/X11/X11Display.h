// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_X11DISPLAY_H
#define RUBRIC_X11DISPLAY_H

#include <draw/Display.h>
#include <xpp/xpp.hpp>
#include "X11EventHandler.h"

namespace rubric::draw::x11 {

    class X11Display : public rubric::draw::Display {

    public:

        explicit X11Display(Context &);

        std::shared_ptr<Surface> createSurface(SurfaceType, std::shared_ptr<Surface>, int, int, int, int) override;

        EventHandler<x::connection &> eventHandler;

    private:
        x::connection connection;
        x::registry registry;

    };

}

#endif //RUBRIC_X11DISPLAY_H
