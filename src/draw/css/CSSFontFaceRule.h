/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * (C) 2002-2003 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2002, 2006, 2008, 2012 Apple Inc. All rights reserved.
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

#include "CSSRule.h"

namespace WebCore {

class CSSStyleDeclaration;
class StyleRuleFontFace;
class StyleRuleCSSStyleDeclaration;

class CSSFontFaceRule final : public CSSRule {
public:
    static ref_ptr<CSSFontFaceRule> create(StyleRuleFontFace& rule, CSSStyleSheet* sheet) { return ref_ptr<CSSFontFaceRule>(rule, sheet); }

    virtual ~CSSFontFaceRule();

    CSSStyleDeclaration& style();

private:
    CSSFontFaceRule(StyleRuleFontFace&, CSSStyleSheet* parent);

    CSSRule::Type type() const final { return FONT_FACE_RULE; }
    std::string cssText() const final;
    void reattach(StyleRuleBase&) final;

    ref_ptr<StyleRuleFontFace> m_fontFaceRule;
    std::shared_ptr<StyleRuleCSSStyleDeclaration> m_propertiesCSSOMWrapper;
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_RULE(CSSFontFaceRule, CSSRule::FONT_FACE_RULE)
