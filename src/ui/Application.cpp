// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "ui/Application.h"
#include "draw/Context.h"

using namespace rubric;
using namespace rubric::ui;
using namespace rubric::draw;

Application::Application(const std::string & id):
appId(id),
context(Context::getInstance()) { }

const std::string & Application::getAppId() const {
    return appId;
}

int Application::run() {
    return EXIT_SUCCESS;
}

void Application::addWindow(std::shared_ptr<Window> window) {
    windows.push_back(window);
}

const auto & Application::getWindows() const {
    return windows;
}

void Application::quit() {

}

void Application::startUp() {

}

void Application::shutDown() {

}