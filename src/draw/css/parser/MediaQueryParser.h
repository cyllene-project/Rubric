// Copyright 2014 The Chromium Authors. All rights reserved.
// Copyright (C) 2016 Apple Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CSSParserToken.h"
#include "CSSParserTokenRange.h"
#include "MediaQuery.h"
#include "MediaQueryBlockWatcher.h"
#include "MediaQueryExpression.h"
#include "MediaQueryParserContext.h"
#include <vector>

namespace WebCore {

class MediaQuerySet;
struct CSSParserContext;

class MediaQueryParser {
    MediaQueryParser(const MediaQueryParser&) = delete;
        MediaQueryParser& operator=(const MediaQueryParser&) = delete;
public:
    static std::shared_ptr<MediaQuerySet> parseMediaQuerySet(const std::string&, MediaQueryParserContext);
    static std::shared_ptr<MediaQuerySet> parseMediaQuerySet(CSSParserTokenRange, MediaQueryParserContext);
    static std::shared_ptr<MediaQuerySet> parseMediaCondition(CSSParserTokenRange, MediaQueryParserContext);

private:
    enum ParserType {
        MediaQuerySetParser,
        MediaConditionParser,
    };

    MediaQueryParser(ParserType, MediaQueryParserContext);
    virtual ~MediaQueryParser();

    std::shared_ptr<MediaQuerySet> parseInternal(CSSParserTokenRange);

    void processToken(const CSSParserToken&, CSSParserTokenRange&);

    void readRestrictor(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void readMediaNot(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void readMediaType(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void readAnd(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void readFeatureStart(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void readFeature(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void readFeatureColon(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void readFeatureValue(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void readFeatureEnd(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void skipUntilComma(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void skipUntilBlockEnd(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);
    void done(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);

    using State = void (MediaQueryParser::*)(CSSParserTokenType, const CSSParserToken&, CSSParserTokenRange&);

    void setStateAndRestrict(State, MediaQuery::Restrictor);
    void handleBlocks(const CSSParserToken&);
    
    void commitMediaQuery();

    class MediaQueryData {
    public:
        explicit MediaQueryData(MediaQueryParserContext);
        MediaQueryData(const MediaQueryData&) = delete;
        MediaQueryData& operator=(const MediaQueryData&) = delete;
        void clear();
        void addExpression(CSSParserTokenRange&);
        bool lastExpressionValid();
        void removeLastExpression();
        void setMediaType(const std::string& mediaType) { m_mediaType = mediaType; }

        MediaQuery::Restrictor restrictor() const { return m_restrictor; }
        std::vector<MediaQueryExpression>& expressions() { return m_expressions; }
        const Optional<std::string>& mediaType() const { return m_mediaType; }

        bool currentMediaQueryChanged() const
        {
            return (m_restrictor != MediaQuery::None || m_mediaType || !m_expressions.empty());
        }
        MediaQuery::Restrictor restrictor() { return m_restrictor; }

        void setRestrictor(MediaQuery::Restrictor restrictor) { m_restrictor = restrictor; }

        void setMediaFeature(const std::string& str) { m_mediaFeature = str; }

        void setMediaQueryParserContext(MediaQueryParserContext context) { m_context = context; }

    private:
        MediaQuery::Restrictor m_restrictor { MediaQuery::None };
        Optional<std::string> m_mediaType;
        std::vector<MediaQueryExpression> m_expressions;
        std::string m_mediaFeature;
        MediaQueryParserContext m_context;
    };

    State m_state;
    ParserType m_parserType;
    MediaQueryData m_mediaQueryData;
    std::shared_ptr<MediaQuerySet> m_querySet;
    MediaQueryBlockWatcher m_blockWatcher;

    const static State ReadRestrictor;
    const static State ReadMediaNot;
    const static State ReadMediaType;
    const static State ReadAnd;
    const static State ReadFeatureStart;
    const static State ReadFeature;
    const static State ReadFeatureColon;
    const static State ReadFeatureValue;
    const static State ReadFeatureEnd;
    const static State SkipUntilComma;
    const static State SkipUntilBlockEnd;
    const static State Done;
};

} // namespace WebCore
