// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_ITEM_H
#define RUBRIC_ITEM_H

namespace rubric::draw::text {

    class Analysis;

    struct Item {

        int offset;
        int length;
        int numChars;
        Analysis *analysis;
    };
}

#endif //RUBRIC_ITEM_H
