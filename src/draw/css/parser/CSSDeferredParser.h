/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "CSSParserContext.h"
#include <vector>
#include <memory>

#include "ref_ptr.h"
using namespace rubric::draw::css;

namespace WebCore {

class CSSParserTokenRange;
class ImmutableStyleProperties;
class StyleRuleKeyframes;
class StyleRuleBase;
class StyleSheetContents;

class CSSDeferredParser {
public:
    static ref_ptr<CSSDeferredParser> create(const CSSParserContext& parserContext, const std::string& sheetText, StyleSheetContents& styleSheet)
    {
        return ref_ptr<CSSDeferredParser>(parserContext, sheetText, styleSheet);
    }

    const CSSParserContext& context() const { return m_context; }
    StyleSheetContents* styleSheet() const;

    ref_ptr<ImmutableStyleProperties> parseDeclaration(const CSSParserTokenRange&);
    void parseRuleList(const CSSParserTokenRange&, std::vector<std::shared_ptr<StyleRuleBase>>&);
    void parseKeyframeList(const CSSParserTokenRange&, StyleRuleKeyframes&);

    void adoptTokenizerEscapedStrings(std::vector<std::string>&& strings) { m_escapedStrings = std::move(strings); }

private:
    CSSDeferredParser(const CSSParserContext&, const std::string&, StyleSheetContents&);
    
    std::vector<std::string> m_escapedStrings;
    CSSParserContext m_context;
    
    std::string m_sheetText;

    std::weak_ptr<StyleSheetContents> m_styleSheet;
};

} // namespace WebCore
