// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_DISPLAYMANAGER_H
#define RUBRIC_DISPLAYMANAGER_H

#include <string>
#include "Display.h"

namespace rubric {
    namespace draw {

        class DisplayManager {


        public:
            Display openDisplay(std::string);
        };

    }
}

#endif //RUBRIC_DISPLAYMANAGER_H
