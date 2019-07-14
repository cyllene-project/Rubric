// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_CSSPARSER_H
#define RUBRIC_CSSPARSER_H

#include "CSSParserContext.h"

namespace rubric::draw::css {

    class CSSParser {

    public:
        CSSParser(const CSSParserContext & context) noexcept;

        ~CSSParser() = default;

        void parseStyleSheet();


    };

}
#endif //RUBRIC_CSSPARSER_H
