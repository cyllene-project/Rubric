// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_X11BACKEND_H
#define RUBRIC_X11BACKEND_H

#include "../Backend.h"

namespace rubric::draw::x11 {

class X11Backend : public Backend {

public:
    std::unique_ptr<Display> open(rubric::draw::Context &, const std::string &) const override;
    std::string getName() const override;

};

}

#endif //RUBRIC_X11BACKEND_H
