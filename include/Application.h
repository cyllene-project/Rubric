// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_APPLICATION_H
#define RUBRIC_APPLICATION_H

#include <string>
#include "Object.h"

namespace rubric {

    class Application: Object {

    public:
        explicit Application(const std::string &);

        std::string getAppId() const;

    private:
        std::string appId;
    };

}

#endif //RUBRIC_APPLICATION_H
