// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_VIEW_H
#define RUBRIC_VIEW_H

#include "Object.h"

namespace rubric {

    class View: public Object {


    public:
        virtual void addSubView(View & subView) = 0;

    };

}
#endif //RUBRIC_VIEW_H
