/*
 * Copyright (c) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "StyleRule.h"
#include <utility>


#include <vector>namespace WebCore {

class StyleRuleBase;

struct SourceRange {
    SourceRange();
    SourceRange(unsigned start, unsigned end);
    unsigned length() const;

    unsigned start;
    unsigned end;
};

struct CSSPropertySourceData {
    CSSPropertySourceData(const std::string& name, const std::string& value, bool important, bool disabled, bool parsedOk, const SourceRange&);
    CSSPropertySourceData(const CSSPropertySourceData& other);
    CSSPropertySourceData();

    std::string toString() const;
    unsigned hash() const;

    std::string name;
    std::string value;
    bool important;
    bool disabled;
    bool parsedOk;
    SourceRange range;
};

struct CSSStyleSourceData : public RefCounted<CSSStyleSourceData> {
    static Ref<CSSStyleSourceData> create()
    {
        return adoptRef(*new CSSStyleSourceData);
    }

    std::vector<CSSPropertySourceData> propertyData;
};

struct CSSRuleSourceData;
typedef std::vector<Ref<CSSRuleSourceData>> RuleSourceDataList;
typedef std::vector<SourceRange> SelectorRangeList;

struct CSSRuleSourceData : public RefCounted<CSSRuleSourceData> {
    static Ref<CSSRuleSourceData> create(StyleRule::Type type)
    {
        return adoptRef(*new CSSRuleSourceData(type));
    }

    static Ref<CSSRuleSourceData> createUnknown()
    {
        return adoptRef(*new CSSRuleSourceData(StyleRule::Unknown));
    }

    CSSRuleSourceData(StyleRule::Type type)
        : type(type)
    {
        if (type == StyleRule::Style || type == StyleRule::FontFace || type == StyleRule::Page)
            styleSourceData = CSSStyleSourceData::create();
    }

    StyleRule::Type type;

    // Range of the selector list in the enclosing source.
    SourceRange ruleHeaderRange;

    // Range of the rule body (e.g. style text for style rules) in the enclosing source.
    SourceRange ruleBodyRange;

    // Only for CSSStyleRules.
    SelectorRangeList selectorRanges;

    // Only for CSSStyleRules, CSSFontFaceRules, and CSSPageRules.
    RefPtr<CSSStyleSourceData> styleSourceData;

    // Only for CSSMediaRules.
    RuleSourceDataList childRules;
};

} // namespace WebCore
