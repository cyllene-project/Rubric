// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_URL_H
#define RUBRIC_URL_H

#include <string>

namespace rubric {
    class URL {

    public:
        explicit URL();

        URL(const URL&, const std::string &);

        bool empty() const noexcept;
    };
}

#endif //RUBRIC_URL_H
