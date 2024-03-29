/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * (C) 2002-2003 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2002, 2006, 2008, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Samuel Weinig (sam@webkit.org)
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

#include "CSSGroupingRule.h"

namespace WebCore {

class MediaList;
class MediaQuerySet;
class StyleRuleMedia;

class CSSMediaRule final : public CSSGroupingRule {
public:
    static ref_ptr<CSSMediaRule> create(StyleRuleMedia& rule, CSSStyleSheet* sheet) { return ref_ptr<CSSMediaRule>(rule, sheet); }

    virtual ~CSSMediaRule();

    MediaList* media() const;

private:
    CSSMediaRule(StyleRuleMedia&, CSSStyleSheet*);

    CSSRule::Type type() const final { return MEDIA_RULE; }
    void reattach(StyleRuleBase&) final;
    std::string cssText() const final;

    MediaQuerySet* mediaQueries() const;
    
    mutable std::shared_ptr<MediaList> m_mediaCSSOMWrapper;
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_RULE(CSSMediaRule, CSSRule::MEDIA_RULE)
