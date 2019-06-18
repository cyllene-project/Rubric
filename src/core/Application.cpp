// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "Application.h"

using namespace rubric;

Application::Application(const std::string & id):
appId(id) {

}

std::string Application::getAppId() const {
    return appId;
}

int Application::run() {
    return EXIT_SUCCESS;
}

void Application::addWindow(std::shared_ptr<rubric::ui::Window> window) {
    windows.push_back(window);
}

const std::vector<std::shared_ptr<rubric::ui::Window>>& Application::getWindows() const {
    return windows;
}