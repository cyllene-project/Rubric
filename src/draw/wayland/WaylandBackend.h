// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_WAYLANDBACKEND_H
#define RUBRIC_WAYLANDBACKEND_H

#include "../Backend.h"

namespace rubric::draw::wayland {

    class WaylandBackend: Backend {

    public:
        std::unique_ptr<Display> open(std::string) const override;
        std::string get_name() const override;

    };
}

#endif //RUBRIC_WAYLANDBACKEND_H
