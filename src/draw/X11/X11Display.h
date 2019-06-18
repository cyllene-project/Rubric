// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_X11DISPLAY_H
#define RUBRIC_X11DISPLAY_H

#include <Display.h>
#include <X11/Xlib.h>

namespace rubric::draw::x11 {

class X11Display : public rubric::draw::Display{

    public:
        explicit X11Display(::Display *xdisplay);

        std::shared_ptr<Surface> createSurface() const override;

    private:
        ::Display * xDisplay;
    };

}

#endif //RUBRIC_X11DISPLAY_H
