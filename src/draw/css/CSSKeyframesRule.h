/*
 * Copyright (C) 2007, 2008, 2012, 2014 Apple Inc. All rights reserved.
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

#include "CSSRule.h"
#include "StyleRule.h"
#include <memory>


namespace WebCore {

class CSSRuleList;
class StyleRuleKeyframe;
class CSSKeyframeRule;

class StyleRuleKeyframes final : public StyleRuleBase {
public:
    static std::reference_wrapper<StyleRuleKeyframes> create(const std::atomic<std::string>& name) { return adoptRef(*new StyleRuleKeyframes(name)); }
    static std::reference_wrapper<StyleRuleKeyframes> create(const std::atomic<std::string>& name, std::unique_ptr<DeferredStyleGroupRuleList>&& deferredRules) { return adoptRef(*new StyleRuleKeyframes(name, std::move(deferredRules))); }
    
    ~StyleRuleKeyframes();
    
    const std::vector<std::reference_wrapper<StyleRuleKeyframe>>& keyframes() const;
    const std::vector<std::reference_wrapper<StyleRuleKeyframe>>* keyframesWithoutDeferredParsing() const
    {
        return !m_deferredRules ? &m_keyframes : nullptr;
    }

    void parserAppendKeyframe(std::shared_ptr<StyleRuleKeyframe>&&);
    void wrapperAppendKeyframe(std::reference_wrapper<StyleRuleKeyframe>&&);
    void wrapperRemoveKeyframe(unsigned);

    const std::atomic<std::string>& name() const { return m_name; }
    void setName(const std::atomic<std::string>& name) { m_name = name; }

    size_t findKeyframeIndex(const std::string& key) const;

    std::reference_wrapper<StyleRuleKeyframes> copy() const { return adoptRef(*new StyleRuleKeyframes(*this)); }

private:
    StyleRuleKeyframes(const std::atomic<std::string>&);
    StyleRuleKeyframes(const std::atomic<std::string>&, std::unique_ptr<DeferredStyleGroupRuleList>&&);
    StyleRuleKeyframes(const StyleRuleKeyframes&);

    void parseDeferredRulesIfNeeded() const;
    
    mutable std::vector<std::reference_wrapper<StyleRuleKeyframe>> m_keyframes;
    AtomString m_name;
    
    mutable std::unique_ptr<DeferredStyleGroupRuleList> m_deferredRules;
};

class CSSKeyframesRule final : public CSSRule {
public:
    static std::reference_wrapper<CSSKeyframesRule> create(StyleRuleKeyframes& rule, CSSStyleSheet* sheet) { return adoptRef(*new CSSKeyframesRule(rule, sheet)); }

    virtual ~CSSKeyframesRule();

    CSSRule::Type type() const final { return KEYFRAMES_RULE; }
    std::string cssText() const final;
    void reattach(StyleRuleBase&) final;

    std::string name() const { return m_keyframesRule->name(); }
    void setName(const std::string&);

    CSSRuleList& cssRules();

    void insertRule(const std::string& rule);
    void appendRule(const std::string& rule);
    void deleteRule(const std::string& key);
    CSSKeyframeRule* findRule(const std::string& key);

    // For IndexedGetter and CSSRuleList.
    unsigned length() const;
    CSSKeyframeRule* item(unsigned index) const;

private:
    CSSKeyframesRule(StyleRuleKeyframes&, CSSStyleSheet* parent);

    std::reference_wrapper<StyleRuleKeyframes> m_keyframesRule;
    mutable std::vector<std::shared_ptr<CSSKeyframeRule>> m_childRuleCSSOMWrappers;
    mutable std::unique_ptr<CSSRuleList> m_ruleListCSSOMWrapper;
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_RULE(CSSKeyframesRule, CSSRule::KEYFRAMES_RULE)

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleRuleKeyframes)
    static bool isType(const WebCore::StyleRuleBase& rule) { return rule.isKeyframesRule(); }
SPECIALIZE_TYPE_TRAITS_END()
