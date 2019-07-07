// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_CONTAINER_H
#define RUBRIC_CONTAINER_H

#include <ui/View.h>

namespace rubric::ui {

    class Container: public View {

    public:
        std::shared_ptr<const Type> & getType() const override;

    };
}

#endif //RUBRIC_CONTAINER_H
