// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_APPLICATION_H
#define RUBRIC_APPLICATION_H

#include <string>
#include <vector>
#include <draw/Context.h>
#include "core/Object.h"
#include "ui/Window.h"

namespace rubric::ui {

    class Application: Object {

    public:
        explicit Application(const std::string &);

        const std::string & getAppId() const;

        int run();

        void quit();

        void addWindow(std::shared_ptr<Window>);

        virtual void startUp();

        virtual void shutDown();

        const auto & getWindows() const;


    private:
        const std::string appId;
        draw::Context & context;
        std::vector<std::shared_ptr<Window>> windows;
    };

}

#endif //RUBRIC_APPLICATION_H
