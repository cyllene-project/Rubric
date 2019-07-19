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

        std::string url;
    };
}

namespace std {
    template<> struct hash<rubric::URL> {
        typedef rubric::URL argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept  {
            return std::hash<std::string>{}(s.url);
        }
    };
}


#endif //RUBRIC_URL_H
