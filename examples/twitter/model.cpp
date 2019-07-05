// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "model.h"

namespace model {

    bool operator<(const TimeRange &lhs, const TimeRange &rhs) {
        return lhs.begin < rhs.begin && lhs.end < rhs.end;
    }

    Reducer noop = Reducer([](Model& m){return std::move(m);});

    int idx = 0;

}