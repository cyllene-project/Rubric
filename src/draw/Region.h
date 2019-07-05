// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_REGION_H
#define RUBRIC_REGION_H

namespace rubric::draw {

    struct Box {
        int x1;
        int y1;
        int x2;
        int y2;
    };

    struct Region {
        Box extents;
    };

}


#endif //RUBRIC_REGION_H
