// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_CSSPARSERTOKEN_H
#define RUBRIC_CSSPARSERTOKEN_H

namespace rubric::draw::css {

    enum class CSSParserTokenType {
        IdentToken,
        FunctionToken,
        AtKeywordToken,
        HashToken,
        UrlToken,
        BadUrlToken,
        DelimiterToken,
        NumberToken,
        PercentageToken,
        DimensionToken,
        IncludeMatchToken,
        DashMatchToken,
        PrefixMatchToken,
        SuffixMatchToken,
        SubstringMatchToken,
        ColumnToken,
        UnicodeRangeToken,
        WhitespaceToken,
        CDOToken,
        CDCToken,
        ColonToken,
        SemicolonToken,
        CommaToken,
        LeftParenthesisToken,
        RightParenthesisToken,
        LeftBracketToken,
        RightBracketToken,
        LeftBraceToken,
        RightBraceToken,
        StringToken,
        BadStringToken,
        EOFToken,
        CommentToken
    };

    enum class NumericSign {
        NoSign,
        PlusSign,
        MinusSign
    };

    enum class NumericValueType {
        IntegerValueType,
        NumberValueType
    };

    enum class HashTokenType {
        HashTokenId,
        HashTokenUnrestricted,
    };

    class CSSParserToken {

    public:
        enum BlockType {
            NotBlock,
            BlockStart,
            BlockEnd,
        };

        CSSParserToken(CSSParserTokenType, BlockType = NotBlock);

        bool operator==(const CSSParserToken& other) const;
        bool operator!=(const CSSParserToken& other) const { return !(*this == other); }




    };

}

#endif //RUBRIC_CSSPARSERTOKEN_H
