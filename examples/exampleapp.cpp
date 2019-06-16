// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)


#include "Rubric.h"
#include "Application.h"

using namespace rubric;

class ExampleApp: public Application {

public:
    ExampleApp(): Application("org.rubric.exampleapp") { }

    int run() const {
        return 0;
    }
};

int main(int argc, char* argv[]) {

    const auto app = ExampleApp();

    return app.run();
}