// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_CONTROL_H
#define RUBRIC_CONTROL_H

#include <ui/View.h>
#include <Rubric.h>

namespace rubric::ui {

    class Control: public View {

    public:
        std::shared_ptr<const Type> & getType() const override;
    };

}
#endif //RUBRIC_CONTROL_H
