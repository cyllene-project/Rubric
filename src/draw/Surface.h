// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_SURFACE_H
#define RUBRIC_SURFACE_H

#include <memory>

namespace rubric::draw {

    class Display;

    class Surface {


    public:
        virtual Display & getDisplay() = 0;
        virtual void show();
        virtual void hide();
        virtual void resize(int, int);

    };

}
#endif //RUBRIC_SURFACE_H
