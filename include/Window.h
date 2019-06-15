//
// Created by bizarro on 6/14/19.
//

#ifndef RUBRIC_WINDOW_H
#define RUBRIC_WINDOW_H

#include "Object.h"

namespace rubric {

    enum class WindowType {};

    class Window: public Object {

    public:
        explicit Window(WindowType);

    };
}

#endif //RUBRIC_WINDOW_H
