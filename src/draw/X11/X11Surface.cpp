// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "X11Surface.h"
#include <draw/Context.h>
#include <core/Event.h>
#include "draw/X11/X11EventHandler.h"

using namespace rubric;
using namespace rubric::draw;
using namespace rubric::draw::x11;

X11Surface::X11Surface(X11Display &display, x::connection &c, SurfaceType surfaceType, int x, int y, int width, int height) :
xDisplay(display),
connection(c) {

    auto bus = display.getContext().getFrameBus();

    bus.subscribe([&](int n) {
        this->tick();
    });

    auto screen = connection.screen_of_display(0);
    auto window = connection.generate_id();

    uint32_t mask = XCB_CW_EVENT_MASK;
    uint32_t values[1] = { XCB_EVENT_MASK_EXPOSURE      | XCB_EVENT_MASK_BUTTON_PRESS |
                           XCB_EVENT_MASK_BUTTON_RELEASE| XCB_EVENT_MASK_POINTER_MOTION |
                           XCB_EVENT_MASK_ENTER_WINDOW  | XCB_EVENT_MASK_LEAVE_WINDOW |
                           XCB_EVENT_MASK_KEY_PRESS     | XCB_EVENT_MASK_KEY_RELEASE };

    xpp::x::create_window(connection,
            XCB_COPY_FROM_PARENT,
            window, screen->root,
            x, y, width, height, 10,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            screen->root_visual,
            mask, values);

    connection.map_window(window);
    connection.flush();

}

rubric::draw::Display & X11Surface::getDisplay() {
    return xDisplay;
}

void X11Surface::setTitle(const std::string & surfaceTitle) {
    title = surfaceTitle;
}