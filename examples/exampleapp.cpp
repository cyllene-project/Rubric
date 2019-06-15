// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)


#include "../include/Rubric.h"
#include "../include/Application.h"

using namespace rubric;

class ExampleApp: public Application {


public:
    int run() const {
        return 0;
    }
};

int main(int argc, char* argv[]) {

    const auto app = ExampleApp();

    return app.run();
}