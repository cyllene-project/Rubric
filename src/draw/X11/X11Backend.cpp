// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "X11Backend.h"
#include <X11/Xlibint.h>
#include <X11/Xlib.h>

using namespace rubric::draw::x11;
using namespace rubric;


static void internal_connection_watch(Display *display,
        XPointer arg,
        int fd,
        int opening,
        XPointer *watch_data) {

}


std::unique_ptr<draw::Display> X11Backend::open(std::string displayName) const {

    XInitThreads();

    auto xdisplay = XOpenDisplay(displayName.c_str());

    if (!xdisplay)
        throw std::runtime_error("Error opening X Display");

    XAddConnectionWatch(xdisplay, internal_connection_watch, nullptr);


}

std::string X11Backend::get_name() const {
    return "x11";
}