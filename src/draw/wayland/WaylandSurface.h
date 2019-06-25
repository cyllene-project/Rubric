// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_WAYLANDSURFACE_H
#define RUBRIC_WAYLANDSURFACE_H

#include <draw/Surface.h>
#include "WaylandDisplay.h"

namespace rubric::draw::wayland {

    class WaylandSurface: public Surface {

    public:
        explicit WaylandSurface(WaylandDisplay &);
        Display & getDisplay() override;

    private:
        WaylandDisplay & wDisplay;

    };

}

#endif //RUBRIC_WAYLANDSURFACE_H
