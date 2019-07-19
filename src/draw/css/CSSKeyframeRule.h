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
#include "StyleProperties.h"
#include "StyleRule.h"

namespace WebCore {

class CSSStyleDeclaration;
class StyleRuleCSSStyleDeclaration;
class CSSKeyframesRule;

class StyleRuleKeyframe final : public StyleRuleBase {
public:
    static ref_ptr<StyleRuleKeyframe> create(ref_ptr<StyleProperties>&& properties)
    {
        return ref_ptr<StyleRuleKeyframe>(std::move(properties));
    }

    static ref_ptr<StyleRuleKeyframe> create(std::unique_ptr<std::vector<double>> keys, ref_ptr<StyleProperties>&& properties)
    {
        return ref_ptr<StyleRuleKeyframe>(std::move(keys), std::move(properties));
    }
    ~StyleRuleKeyframe();

    std::string keyText() const;
    bool setKeyText(const std::string&);
    void setKey(double key)
    {
        assert(m_keys.isEmpty());
        m_keys.clear();
        m_keys.append(key);
    }

    const std::vector<double>& keys() const { return m_keys; };

    const StyleProperties& properties() const { return m_properties; }
    MutableStyleProperties& mutableProperties();

    std::string cssText() const;

private:
    explicit StyleRuleKeyframe(ref_ptr<StyleProperties>&&);
    StyleRuleKeyframe(std::unique_ptr<std::vector<double>>, ref_ptr<StyleProperties>&&);

    ref_ptr<StyleProperties> m_properties;
    std::vector<double> m_keys;
};

class CSSKeyframeRule final : public CSSRule {
public:
    virtual ~CSSKeyframeRule();

    std::string cssText() const final { return m_keyframe->cssText(); }
    void reattach(StyleRuleBase&) final;

    std::string keyText() const { return m_keyframe->keyText(); }
    void setKeyText(const std::string& text) { m_keyframe->setKeyText(text); }

    CSSStyleDeclaration& style();

private:
    CSSKeyframeRule(StyleRuleKeyframe&, CSSKeyframesRule* parent);

    CSSRule::Type type() const final { return KEYFRAME_RULE; }

    ref_ptr<StyleRuleKeyframe> m_keyframe;
    mutable std::shared_ptr<StyleRuleCSSStyleDeclaration> m_propertiesCSSOMWrapper;
    
    friend class CSSKeyframesRule;
};

} // namespace WebCore
