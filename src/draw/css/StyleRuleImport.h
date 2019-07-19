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

//#include "CachedResourceHandle.h"
//#include "CachedStyleSheetClient.h"
#include "StyleRule.h"

namespace WebCore {

class CachedCSSStyleSheet;
class MediaQuerySet;
class StyleSheetContents;

class StyleRuleImport final : public StyleRuleBase {

public:
    static ref_ptr<StyleRuleImport> create(const std::string& href, ref_ptr<MediaQuerySet>&&);

    ~StyleRuleImport();
    
    StyleSheetContents* parentStyleSheet() const { return m_parentStyleSheet; }
    void setParentStyleSheet(StyleSheetContents* sheet) { assert(sheet); m_parentStyleSheet = sheet; }
    void clearParentStyleSheet() { m_parentStyleSheet = 0; }

    std::string href() const { return m_strHref; }
    StyleSheetContents* styleSheet() const { return m_styleSheet.get(); }

    bool isLoading() const;
    MediaQuerySet* mediaQueries() const { return m_mediaQueries.get(); }

    void requestStyleSheet();
    const CachedCSSStyleSheet* cachedCSSStyleSheet() const { return m_cachedSheet.get(); }

private:
    // NOTE: We put the CachedStyleSheetClient in a member instead of inheriting from it
    // to avoid adding a vptr to StyleRuleImport.
class ImportedStyleSheetClient final : public CachedStyleSheetClient {
    public:
        ImportedStyleSheetClient(StyleRuleImport* ownerRule) : m_ownerRule(ownerRule) { }
        virtual ~ImportedStyleSheetClient() = default;
        void setCSSStyleSheet(const std::string& href, const URL& baseURL, const std::string& charset, const CachedCSSStyleSheet* sheet) final
        {
            m_ownerRule->setCSSStyleSheet(href, baseURL, charset, sheet);
        }
    private:
        StyleRuleImport* m_ownerRule;
    };

    void setCSSStyleSheet(const std::string& href, const URL& baseURL, const std::string& charset, const CachedCSSStyleSheet*);
    friend class ImportedStyleSheetClient;

    StyleRuleImport(const std::string& href, ref_ptr<MediaQuerySet>&&);

    StyleSheetContents* m_parentStyleSheet;

    ImportedStyleSheetClient m_styleSheetClient;
    std::string m_strHref;
    std::shared_ptr<MediaQuerySet> m_mediaQueries;
    std::shared_ptr<StyleSheetContents> m_styleSheet;
    CachedResourceHandle<CachedCSSStyleSheet> m_cachedSheet;
    bool m_loading;
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRuleImport)
    static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isImportRule(); }
SPECIALIZE_TYPE_TRAITS_END()
