// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_DISPLAYMANAGER_H
#define RUBRIC_DISPLAYMANAGER_H

#include <string>
#include <memory>
#include "Display.h"

namespace rubric::draw {

    class DisplayManager {

    public:
        void openDisplay(std::string);
        std::shared_ptr<Display> getDefaultDisplay() const;
        void setDefaultDisplay(std::shared_ptr<Display>);

    private:
        std::shared_ptr<Display> defaultDisplay = nullptr;;
    };

}

#endif //RUBRIC_DISPLAYMANAGER_H
