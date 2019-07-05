// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "TwitterApp.h"

int main(int argc, char *argv[]) {
    auto app = TwitterApp("org.rubric.Twitter");
    return app.run();
}