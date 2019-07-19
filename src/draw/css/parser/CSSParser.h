/*
 * Copyright (C) 2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004-2010, 2015 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2009 - 2010  Torch Mobile (Beijing) Co. Ltd. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#pragma once

#include "CSSParserContext.h"
#include "CSSRegisteredCustomProperty.h"
#include "CSSValue.h"
//#include "WritingMode.h"

namespace WebCore {

struct ApplyCascadedPropertyState;
class CSSParserObserver;
class CSSSelectorList;
class Color;
class Element;
class ImmutableStyleProperties;
class MutableStyleProperties;
class StyleRuleBase;
class StyleRuleKeyframe;
class StyleSheetContents;
class RenderStyle;

class CSSParser {
public:
    enum class ParseResult {
        Changed,
        Unchanged,
        Error
    };

    explicit CSSParser(const CSSParserContext &);
    ~CSSParser();

    enum class RuleParsing { Normal, Deferred };
    void parseSheet(StyleSheetContents*, const std::string&, RuleParsing = RuleParsing::Normal);
    
    static std::shared_ptr<StyleRuleBase> parseRule(const CSSParserContext&, StyleSheetContents*, const std::string&);
    
    std::shared_ptr<StyleRuleKeyframe> parseKeyframeRule(const std::string&);
    static std::unique_ptr<std::vector<double>> parseKeyframeKeyList(const std::string&);
    
    bool parseSupportsCondition(const std::string&);

    static void parseSheetForInspector(const CSSParserContext&, StyleSheetContents*, const std::string&, CSSParserObserver&);
    static void parseDeclarationForInspector(const CSSParserContext&, const std::string&, CSSParserObserver&);

    static ParseResult parseValue(MutableStyleProperties&, CSSPropertyID, const std::string&, bool important, const CSSParserContext&);
    static ParseResult parseCustomPropertyValue(MutableStyleProperties&, const std::atomic<std::string>& propertyName, const std::string&, bool important, const CSSParserContext&);
    
    static std::shared_ptr<CSSValue> parseFontFaceDescriptor(CSSPropertyID, const std::string&, const CSSParserContext&);

    static std::shared_ptr<CSSValue> parseSingleValue(CSSPropertyID, const std::string&, const CSSParserContext& = strictCSSParserContext());

    bool parseDeclaration(MutableStyleProperties&, const std::string&);
    static std::reference_wrapper<ImmutableStyleProperties> parseInlineStyleDeclaration(const std::string&, const Element*);

    void parseSelector(const std::string&, CSSSelectorList&);

    std::shared_ptr<CSSValue> parseValueWithVariableReferences(CSSPropertyID, const CSSValue&, ApplyCascadedPropertyState&);

    static Color parseColor(const std::string&, bool strict = false);
    static Color parseSystemColor(const std::string&, const CSSParserContext*);

private:
    ParseResult parseValue(MutableStyleProperties&, CSSPropertyID, const std::string&, bool important);

    CSSParserContext m_context;
};

} // namespace WebCore
