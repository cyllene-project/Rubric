// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_X11DISPLAY_H
#define RUBRIC_X11DISPLAY_H

#include <draw/Display.h>
#include <xpp/xpp.hpp>

namespace rubric::draw::x11 {

    namespace x {
        typedef xpp::connection<> connection;
        typedef xpp::event::registry<connection &> registry;

        typedef xpp::font<connection &> font;
        typedef xpp::cursor<connection &> cursor;
        typedef xpp::window<connection &> window;

        typedef xpp::x::event::key_press<connection &> key_press;
        typedef xpp::x::event::key_release<connection &> key_release;
        typedef xpp::x::event::button_press<connection &> button_press;
    };


    class X11Display : public rubric::draw::Display{

    public:

        explicit X11Display(x::connection &);

        std::unique_ptr<Surface> createSurface() override;

    private:

        x::connection & connection;

    };

}

#endif //RUBRIC_X11DISPLAY_H
