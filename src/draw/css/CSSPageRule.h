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
class CSSStyleSheet;
class StyleRulePage;
class StyleRuleCSSStyleDeclaration;

class CSSPageRule final : public CSSRule {
public:
    static ref_ptr<CSSPageRule> create(StyleRulePage& rule, CSSStyleSheet* sheet) { return ref_ptr<CSSPageRule>(rule, sheet); }

    virtual ~CSSPageRule();

    CSSStyleDeclaration& style();

    std::string selectorText() const;
    void setSelectorText(const std::string&);

private:
    CSSPageRule(StyleRulePage&, CSSStyleSheet*);

    CSSRule::Type type() const final { return PAGE_RULE; }
    std::string cssText() const final;
    void reattach(StyleRuleBase&) final;

    ref_ptr<StyleRulePage> m_pageRule;
    mutable std::shared_ptr<StyleRuleCSSStyleDeclaration> m_propertiesCSSOMWrapper;
};

} // namespace WebCore
