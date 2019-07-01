// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_LAYOUTMANAGER_H
#define RUBRIC_LAYOUTMANAGER_H


namespace rubric::ui {

    class View;

    enum class Orientation { horizontal, vertical };

    class LayoutManager {

    public:
        virtual void measure(View &, Orientation, int, int, int, int, int) = 0;
        virtual void allocate(View &, int, int, int) = 0;


    };
}

#endif //RUBRIC_LAYOUTMANAGER_H
