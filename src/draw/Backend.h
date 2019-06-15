// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_BACKEND_H
#define RUBRIC_BACKEND_H

#include <Display.h>
#include <memory>

namespace rubric::draw {
    class Backend;
}

class rubric::draw::Backend {

public:
    virtual std::unique_ptr<Display> open(std::string name) const = 0;
    virtual std::string get_name() const = 0;

};

#endif //RUBRIC_BACKEND_H
