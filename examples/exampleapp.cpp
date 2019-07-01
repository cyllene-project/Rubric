// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)


#include "Rubric.h"
#include "Application.h"

using namespace rubric;

class ExampleApp: public Application {

public:
    ExampleApp(): Application("org.rubric.exampleapp") { }

};

class TestWindow: public ui::Window {

public:
    TestWindow() : ui::Window() { }

    ~TestWindow() {
        std::cout << "Destroying Window" << std::endl;
    }
};

int main(int argc, char* argv[]) {


    auto & context = rubric::draw::Context::getInstance();
    auto & rl = context.getRunLoop();

    TestWindow window;
    window.title = "Test Window";
    window.show();
    rl.run();
}