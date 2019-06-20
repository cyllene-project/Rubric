// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_OBJECT_H
#define RUBRIC_OBJECT_H

#include <string>
#include <map>
#include <Type.h>
#include <any>

namespace rubric {
    /**
     * Base class for all Rubric objects
     */
    class Object {

    public:
        virtual Type & getType();

        void setProperty(std::string &, std::any &);
    };

}
#endif //RUBRIC_OBJECT_H
