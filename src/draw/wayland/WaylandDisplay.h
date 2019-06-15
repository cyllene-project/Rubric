// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_WAYLANDDISPLAY_H
#define RUBRIC_WAYLANDDISPLAY_H

#include <Display.h>
#include <wayland-client-core.h>

using WLDisplay = struct wl_display*;
using WLRegistry = struct wl_registry*;

namespace rubric::draw {

    class WaylandDisplay: public Display {

    public:
        WaylandDisplay(WLDisplay);

    private:
        WLDisplay wlDisplay;
        WLRegistry wlRegistry;
    };
}


#endif //RUBRIC_WAYLANDDISPLAY_H
