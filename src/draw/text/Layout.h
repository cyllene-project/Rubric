// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_LAYOUT_H
#define RUBRIC_LAYOUT_H

#include <vector>
#include "GlyphItem.h"

namespace rubric::draw::text {

    enum class Alignment { left, center, right };

    enum class Wrap { word, character, word_character };

    enum class Elipsis { none, start, middle, end };

    class Layout {

        using Run = GlyphItem;

        struct Line {
            Layout & layout;
            int start;
            int length;
            std::vector<Run> runs;
            int paragraphStart;
        };


    };

}

#endif //RUBRIC_LAYOUT_H
