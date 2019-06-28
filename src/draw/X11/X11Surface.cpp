// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "X11Surface.h"

using namespace rubric;
using namespace rubric::draw;
using namespace rubric::draw::x11;

X11Surface::X11Surface(X11Display & display, x::connection & c):
xDisplay(display),
connection(c) {
    auto screen = connection.screen_of_display(0);
    auto window = connection.generate_id();
    xpp::x::create_window(connection,
            XCB_COPY_FROM_PARENT,
            window, screen->root,
            0, 0, 150, 150, 10,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            screen->root_visual,
            0, nullptr);

    connection.map_window(window);
    connection.flush();
}

rubric::draw::Display & X11Surface::getDisplay() {
    return xDisplay;
}

void X11Surface::setTitle(const std::string & surfaceTitle) {
    title = surfaceTitle;
}