/*
 * Copyright (C) 2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008, 2009, 2010, 2016 Apple Inc. All rights reserved.
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

#include "CSSParserTokenRange.h"
#include "StyleRule.h"


namespace WebCore {

class CSSProperty;
class CSSValue;
class StylePropertyShorthand;
class StyleSheetContents;
class StyleResolver;
    
// Inputs: PropertyID, isImportant bool, CSSParserTokenRange.
// Outputs: Vector of CSSProperties

class CSSPropertyParser {
    CSSPropertyParser(const CSSPropertyParser&) = delete;
        CSSPropertyParser& operator=(const CSSPropertyParser&) = delete;
public:
    static bool parseValue(CSSPropertyID, bool important,
        const CSSParserTokenRange&, const CSSParserContext&,
        std::vector<CSSProperty, 256>&, StyleRule::Type);

    // Parses a non-shorthand CSS property
    static std::shared_ptr<CSSValue> parseSingleValue(CSSPropertyID, const CSSParserTokenRange&, const CSSParserContext&);
    static bool canParseTypedCustomPropertyValue(const std::string& syntax, const CSSParserTokenRange&, const CSSParserContext&);
    static std::shared_ptr<CSSCustomPropertyValue> parseTypedCustomPropertyValue(const std::string& name, const std::string& syntax, const CSSParserTokenRange&, const StyleResolver&, const CSSParserContext&);
    static void collectParsedCustomPropertyValueDependencies(const std::string& syntax, bool isRoot, std::unordered_set<CSSPropertyID>& dependencies, const CSSParserTokenRange&, const CSSParserContext&);

private:
    CSSPropertyParser(const CSSParserTokenRange&, const CSSParserContext&, std::vector<CSSProperty, 256>*, bool consumeWhitespace = true);

    // FIXME: Rename once the CSSParserValue-based parseValue is removed
    bool parseValueStart(CSSPropertyID, bool important);
    bool consumeCSSWideKeyword(CSSPropertyID, bool important);
    std::shared_ptr<CSSValue> parseSingleValue(CSSPropertyID, CSSPropertyID = CSSPropertyInvalid);
    bool canParseTypedCustomPropertyValue(const std::string& syntax);
    std::shared_ptr<CSSCustomPropertyValue> parseTypedCustomPropertyValue(const std::string& name, const std::string& syntax, const StyleResolver&);
    void collectParsedCustomPropertyValueDependencies(const std::string& syntax, bool isRoot, std::unordered_set<CSSPropertyID>& dependencies);

    bool parseViewportDescriptor(CSSPropertyID propId, bool important);
    bool parseFontFaceDescriptor(CSSPropertyID);

    void addProperty(CSSPropertyID, CSSPropertyID, ref_ptr<CSSValue>&&, bool important, bool implicit = false);
    void addExpandedPropertyForValue(CSSPropertyID propId, ref_ptr<CSSValue>&&, bool);

    bool consumeBorder(std::shared_ptr<CSSValue>& width, std::shared_ptr<CSSValue>& style, std::shared_ptr<CSSValue>& color);

    bool parseShorthand(CSSPropertyID, bool important);
    bool consumeShorthandGreedily(const StylePropertyShorthand&, bool important);
    bool consume2ValueShorthand(const StylePropertyShorthand&, bool important);
    bool consume4ValueShorthand(const StylePropertyShorthand&, bool important);

    // Legacy parsing allows <string>s for animation-name
    bool consumeAnimationShorthand(const StylePropertyShorthand&, bool important);
    bool consumeBackgroundShorthand(const StylePropertyShorthand&, bool important);

    bool consumeColumns(bool important);

    bool consumeGridItemPositionShorthand(CSSPropertyID, bool important);
    bool consumeGridTemplateRowsAndAreasAndColumns(CSSPropertyID, bool important);
    bool consumeGridTemplateShorthand(CSSPropertyID, bool important);
    bool consumeGridShorthand(bool important);
    bool consumeGridAreaShorthand(bool important);

    bool consumePlaceContentShorthand(bool important);
    bool consumePlaceItemsShorthand(bool important);
    bool consumePlaceSelfShorthand(bool important);

    bool consumeFont(bool important);
    bool consumeFontVariantShorthand(bool important);
    bool consumeSystemFont(bool important);

    bool consumeBorderSpacing(bool important);

    // CSS3 Parsing Routines (for properties specific to CSS3)
    bool consumeBorderImage(CSSPropertyID, bool important);

    bool consumeFlex(bool important);

    bool consumeLegacyBreakProperty(CSSPropertyID, bool important);

    bool consumeTransformOrigin(bool important);
    bool consumePerspectiveOrigin(bool important);

private:
    // Inputs:
    CSSParserTokenRange m_range;
    const CSSParserContext& m_context;

    // Outputs:
    std::vector<CSSProperty, 256>* m_parsedProperties;
};

CSSPropertyID cssPropertyID(StringView);
CSSValueID cssValueKeywordID(StringView);
bool isCustomPropertyName(const std::string&);

//#if PLATFORM(IOS_FAMILY)
//void cssPropertyNameIOSAliasing(const char* propertyName, const char*& propertyNameAlias, unsigned& newLength);
//#endif

} // namespace WebCore
