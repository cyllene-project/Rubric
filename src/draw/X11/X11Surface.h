// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_X11SURFACE_H
#define RUBRIC_X11SURFACE_H

#include <draw/Surface.h>
#include "X11Display.h"

namespace rubric::draw::x11 {

    class X11Surface: public Surface {

    public:
        explicit X11Surface(X11Display &, x::connection &, SurfaceType, int, int, int, int);
        Display & getDisplay() override;
        void setTitle(const std::string &) override;

    private:
        X11Display & xDisplay;
        x::connection & connection;
        std::string title;
        bool frameSync = true;

    };

}
#endif //RUBRIC_X11SURFACE_H
