/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * (C) 2002-2003 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2002, 2006, 2008, 2012, 2013 Apple Inc. All rights reserved.
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

#include "CSSSelectorList.h"
//#include "CompiledSelector.h"
#include "StyleProperties.h"

namespace WebCore {

class CSSRule;
class CSSStyleRule;
class CSSStyleSheet;
class MediaQuerySet;
class MutableStyleProperties;
class StyleRuleKeyframe;
class StyleProperties;
class StyleRuleKeyframes;
    
class StyleRuleBase : public WTF::RefCountedBase {

public:
    enum Type {
        Unknown, // Not used.
        Style,
        Charset, // Not used. These are internally strings owned by the style sheet.
        Import,
        Media,
        FontFace,
        Page,
        Keyframes,
        Keyframe, // Not used. These are internally non-rule StyleRuleKeyframe objects.
        Namespace,
        Supports = 12,
    };

    Type type() const { return static_cast<Type>(m_type); }
    
    bool isCharsetRule() const { return type() == Charset; }
    bool isFontFaceRule() const { return type() == FontFace; }
    bool isKeyframesRule() const { return type() == Keyframes; }
    bool isKeyframeRule() const { return type() == Keyframe; }
    bool isNamespaceRule() const { return type() == Namespace; }
    bool isMediaRule() const { return type() == Media; }
    bool isPageRule() const { return type() == Page; }
    bool isStyleRule() const { return type() == Style; }
    bool isSupportsRule() const { return type() == Supports; }
    bool isImportRule() const { return type() == Import; }

    ref_ptr<StyleRuleBase> copy() const;

    void deref()
    {
        if (derefBase())
            destroy();
    }

    // FIXME: There shouldn't be any need for the null parent version.
    ref_ptr<CSSRule> createCSSOMWrapper(CSSStyleSheet* parentSheet = nullptr) const;
    ref_ptr<CSSRule> createCSSOMWrapper(CSSRule* parentRule) const;

protected:
    StyleRuleBase(Type type, bool hasDocumentSecurityOrigin = false)
        : m_type(type)
        , m_hasDocumentSecurityOrigin(hasDocumentSecurityOrigin)
    {
    }

    StyleRuleBase(const StyleRuleBase& o)
        : WTF::RefCountedBase()
        , m_type(o.m_type)
        , m_hasDocumentSecurityOrigin(o.m_hasDocumentSecurityOrigin)
    {
    }

    ~StyleRuleBase() = default;

    bool hasDocumentSecurityOrigin() const { return m_hasDocumentSecurityOrigin; }

private:
    void destroy();
    
    ref_ptr<CSSRule> createCSSOMWrapper(CSSStyleSheet* parentSheet, CSSRule* parentRule) const;

    unsigned m_type : 5;
    // This is only needed to support getMatchedCSSRules.
    unsigned m_hasDocumentSecurityOrigin : 1;
};

class StyleRule final : public StyleRuleBase {

public:
    static ref_ptr<StyleRule> create(ref_ptr<StylePropertiesBase>&& properties, bool hasDocumentSecurityOrigin, CSSSelectorList&& selectors)
    {
        return ref_ptr<StyleRule>(std::move(properties), hasDocumentSecurityOrigin, std::move(selectors));
    }
    
    ~StyleRule();

    const CSSSelectorList& selectorList() const { return m_selectorList; }
    
    const StyleProperties& properties() const;
    MutableStyleProperties& mutableProperties();
    const StyleProperties* propertiesWithoutDeferredParsing() const;

    using StyleRuleBase::hasDocumentSecurityOrigin;

    void wrapperAdoptSelectorList(CSSSelectorList&& selectors)
    {
        m_selectorList = std::move(selectors);
//#if ENABLE(CSS_SELECTOR_JIT)
        m_compiledSelectors = nullptr;
//#endif
    }

    ref_ptr<StyleRule> copy() const { return ref_ptr<StyleRule>(*this); }

    std::vector<std::shared_ptr<StyleRule>> splitIntoMultipleRulesWithMaximumSelectorComponentCount(unsigned) const;

//#if ENABLE(CSS_SELECTOR_JIT)
    CompiledSelector& compiledSelectorForListIndex(unsigned index)
    {
        if (!m_compiledSelectors)
            m_compiledSelectors = makeUniqueArray<CompiledSelector>(m_selectorList.listSize());
        return m_compiledSelectors[index];
    }
    void releaseCompiledSelectors() const
    {
        m_compiledSelectors = nullptr;
    }
//#endif

    static unsigned averageSizeInBytes();

private:
    StyleRule(ref_ptr<StylePropertiesBase>&&, bool hasDocumentSecurityOrigin, CSSSelectorList&&);
    StyleRule(const StyleRule&);

    static ref_ptr<StyleRule> createForSplitting(const std::vector<const CSSSelector*>&, ref_ptr<StyleProperties>&&, bool hasDocumentSecurityOrigin);

    mutable ref_ptr<StylePropertiesBase> m_properties;
    CSSSelectorList m_selectorList;

//#if ENABLE(CSS_SELECTOR_JIT)
    mutable UniqueArray<CompiledSelector> m_compiledSelectors;
//#endif
};

inline const StyleProperties* StyleRule::propertiesWithoutDeferredParsing() const
{
    return m_properties->type() != DeferredPropertiesType ? &downcast<StyleProperties>(m_properties.get()) : nullptr;
}

class StyleRuleFontFace final : public StyleRuleBase {
public:
    static ref_ptr<StyleRuleFontFace> create(ref_ptr<StyleProperties>&& properties) { return ref_ptr<StyleRuleFontFace>(std::move(properties)); }
    
    ~StyleRuleFontFace();

    const StyleProperties& properties() const { return m_properties; }
    MutableStyleProperties& mutableProperties();

    ref_ptr<StyleRuleFontFace> copy() const { return ref_ptr<StyleRuleFontFace>(*this); }

private:
    explicit StyleRuleFontFace(ref_ptr<StyleProperties>&&);
    StyleRuleFontFace(const StyleRuleFontFace&);

    ref_ptr<StyleProperties> m_properties;
};

class StyleRulePage final : public StyleRuleBase {
public:
    static ref_ptr<StyleRulePage> create(ref_ptr<StyleProperties>&& properties, CSSSelectorList&& selectors) { return ref_ptr<StyleRulePage>(std::move(properties), std::move(selectors)); }

    ~StyleRulePage();

    const CSSSelector* selector() const { return m_selectorList.first(); }    
    const StyleProperties& properties() const { return m_properties; }
    MutableStyleProperties& mutableProperties();

    void wrapperAdoptSelectorList(CSSSelectorList&& selectors) { m_selectorList = std::move(selectors); }

    ref_ptr<StyleRulePage> copy() const { return ref_ptr<StyleRulePage>(*this); }

private:
    explicit StyleRulePage(ref_ptr<StyleProperties>&&, CSSSelectorList&&);
    StyleRulePage(const StyleRulePage&);
    
    ref_ptr<StyleProperties> m_properties;
    CSSSelectorList m_selectorList;
};

class DeferredStyleGroupRuleList final {

public:
    DeferredStyleGroupRuleList(const CSSParserTokenRange&, CSSDeferredParser&);
    
    void parseDeferredRules(std::vector<std::shared_ptr<StyleRuleBase>>&);
    void parseDeferredKeyframes(StyleRuleKeyframes&);

private:
    std::vector<CSSParserToken> m_tokens;
    ref_ptr<CSSDeferredParser> m_parser;
};
    
class StyleRuleGroup : public StyleRuleBase {
public:
    const std::vector<std::shared_ptr<StyleRuleBase>>& childRules() const;
    const std::vector<std::shared_ptr<StyleRuleBase>>* childRulesWithoutDeferredParsing() const;

    void wrapperInsertRule(unsigned, ref_ptr<StyleRuleBase>&&);
    void wrapperRemoveRule(unsigned);
    
protected:
    StyleRuleGroup(Type, std::vector<std::shared_ptr<StyleRuleBase>>&);
    StyleRuleGroup(Type, std::unique_ptr<DeferredStyleGroupRuleList>&&);
    StyleRuleGroup(const StyleRuleGroup&);
    
private:
    void parseDeferredRulesIfNeeded() const;

    mutable std::vector<std::shared_ptr<StyleRuleBase>> m_childRules;
    mutable std::unique_ptr<DeferredStyleGroupRuleList> m_deferredRules;
};

inline const std::vector<std::shared_ptr<StyleRuleBase>>* StyleRuleGroup::childRulesWithoutDeferredParsing() const
{
    return !m_deferredRules ? &m_childRules : nullptr;
}

class StyleRuleMedia final : public StyleRuleGroup {
public:
    static ref_ptr<StyleRuleMedia> create(ref_ptr<MediaQuerySet>&& media, std::vector<std::shared_ptr<StyleRuleBase>>& adoptRules)
    {
        return ref_ptr<StyleRuleMedia>(std::move(media), adoptRules);
    }

    static ref_ptr<StyleRuleMedia> create(ref_ptr<MediaQuerySet>&& media, std::unique_ptr<DeferredStyleGroupRuleList>&& deferredChildRules)
    {
        return ref_ptr<StyleRuleMedia>(std::move(media), std::move(deferredChildRules));
    }

    MediaQuerySet* mediaQueries() const { return m_mediaQueries.get(); }

    ref_ptr<StyleRuleMedia> copy() const { return ref_ptr<StyleRuleMedia>(*this); }

private:
    StyleRuleMedia(ref_ptr<MediaQuerySet>&&, std::vector<std::shared_ptr<StyleRuleBase>>& adoptRules);
    StyleRuleMedia(ref_ptr<MediaQuerySet>&&, std::unique_ptr<DeferredStyleGroupRuleList>&&);
    StyleRuleMedia(const StyleRuleMedia&);

    std::shared_ptr<MediaQuerySet> m_mediaQueries;
};

class StyleRuleSupports final : public StyleRuleGroup {
public:
    static ref_ptr<StyleRuleSupports> create(const std::string& conditionText, bool conditionIsSupported, std::vector<std::shared_ptr<StyleRuleBase>>& adoptRules)
    {
        return ref_ptr<StyleRuleSupports>(conditionText, conditionIsSupported, adoptRules);
    }
    
    static ref_ptr<StyleRuleSupports> create(const std::string& conditionText, bool conditionIsSupported, std::unique_ptr<DeferredStyleGroupRuleList>&& deferredChildRules)
    {
        return ref_ptr<StyleRuleSupports>(conditionText, conditionIsSupported, std::move(deferredChildRules));
    }

    std::string conditionText() const { return m_conditionText; }
    bool conditionIsSupported() const { return m_conditionIsSupported; }
    ref_ptr<StyleRuleSupports> copy() const { return ref_ptr<StyleRuleSupports>(*this); }

private:
    StyleRuleSupports(const std::string& conditionText, bool conditionIsSupported, std::vector<std::shared_ptr<StyleRuleBase>>& adoptRules);
    StyleRuleSupports(const std::string& conditionText, bool conditionIsSupported, std::unique_ptr<DeferredStyleGroupRuleList>&&);
    
    StyleRuleSupports(const StyleRuleSupports&);

    std::string m_conditionText;
    bool m_conditionIsSupported;
};

//#if ENABLE(CSS_DEVICE_ADAPTATION)
class StyleRuleViewport final : public StyleRuleBase {
public:
    static ref_ptr<StyleRuleViewport> create(ref_ptr<StyleProperties>&& properties) { return ref_ptr<StyleRuleViewport>(std::move(properties)); }

    ~StyleRuleViewport();

    const StyleProperties& properties() const { return m_properties.get(); }
    MutableStyleProperties& mutableProperties();

    ref_ptr<StyleRuleViewport> copy() const { return ref_ptr<StyleRuleViewport>(*this); }

private:
    explicit StyleRuleViewport(ref_ptr<StyleProperties>&&);
    StyleRuleViewport(const StyleRuleViewport&);

    ref_ptr<StyleProperties> m_properties;
};
//#endif // ENABLE(CSS_DEVICE_ADAPTATION)

// This is only used by the CSS parser.
class StyleRuleCharset final : public StyleRuleBase {
public:
    static ref_ptr<StyleRuleCharset> create() { return ref_ptr<StyleRuleCharset>(); }
    
    ~StyleRuleCharset();
    
    ref_ptr<StyleRuleCharset> copy() const { return ref_ptr<StyleRuleCharset>(*this); }

private:
    explicit StyleRuleCharset();
    StyleRuleCharset(const StyleRuleCharset&);
};

class StyleRuleNamespace final : public StyleRuleBase {
public:
    static ref_ptr<StyleRuleNamespace> create(AtomString prefix, AtomString uri)
    {
        return ref_ptr<StyleRuleNamespace>(prefix, uri);
    }
    
    ~StyleRuleNamespace();

    ref_ptr<StyleRuleNamespace> copy() const { return ref_ptr<StyleRuleNamespace>(*this); }
    
    AtomString prefix() const { return m_prefix; }
    AtomString uri() const { return m_uri; }

private:
    StyleRuleNamespace(AtomString prefix, AtomString uri);
    StyleRuleNamespace(const StyleRuleNamespace&);
    
    AtomString m_prefix;
    AtomString m_uri;
};
    
} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRule)
    static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isStyleRule(); }
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRuleFontFace)
    static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isFontFaceRule(); }
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRuleMedia)
    static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isMediaRule(); }
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRulePage)
    static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isPageRule(); }
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRuleSupports)
    static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isSupportsRule(); }
SPECIALIZE_TYPE_TRAITS_END()

//#if ENABLE(CSS_DEVICE_ADAPTATION)
SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRuleViewport)
    static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isViewportRule(); }
SPECIALIZE_TYPE_TRAITS_END()
//#endif // ENABLE(CSS_DEVICE_ADAPTATION)

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRuleNamespace)
    static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isNamespaceRule(); }
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRuleKeyframe)
static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isKeyframeRule(); }
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRuleCharset)
static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isCharsetRule(); }
SPECIALIZE_TYPE_TRAITS_END()

