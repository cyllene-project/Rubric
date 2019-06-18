// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_APPLICATION_H
#define RUBRIC_APPLICATION_H

#include <string>
#include <vector>
#include "Object.h"
#include "Window.h"

namespace rubric {

    class Application: Object {

    public:
        explicit Application(const std::string &);

        std::string getAppId() const;

        int run();

        void addWindow(std::shared_ptr<rubric::ui::Window>);

        const std::vector<std::shared_ptr<rubric::ui::Window>> & getWindows() const;

    private:
        std::string appId;
        std::vector<std::shared_ptr<rubric::ui::Window>> windows;
    };

}

#endif //RUBRIC_APPLICATION_H
