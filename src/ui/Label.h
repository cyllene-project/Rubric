// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_LABEL_H
#define RUBRIC_LABEL_H

#include <ui/View.h>

namespace rubric::ui {

    class Label: public View {

    public:
        Property<std::string> text;
        Property<bool> wrap { false };
        Property<bool> justify { false };
        Property<int> widthCharacters { -1 };

    private:
        int maxWidthCharacters = -1;
        int lines = -1;
        float xAlign = 0.5;
        float yAlign = 0.5;

    };

}

#endif //RUBRIC_LABEL_H
