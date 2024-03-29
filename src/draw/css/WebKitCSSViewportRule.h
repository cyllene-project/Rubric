/*
 * Copyright (C) 2012 Intel Corporation. All rights reserved.
 * Copyright (C) 2012 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER “AS IS” AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#if ENABLE(CSS_DEVICE_ADAPTATION)

#include "CSSRule.h"

namespace WebCore {

class CSSStyleDeclaration;
class StyleRuleViewport;
class StyleRuleCSSStyleDeclaration;

class WebKitCSSViewportRule final : public CSSRule {
public:
    static ref_ptr<WebKitCSSViewportRule> create(StyleRuleViewport& viewportRule, CSSStyleSheet* sheet)
    {
        return ref_ptr<WebKitCSSViewportRule>(viewportRule, sheet);
    }
    virtual ~WebKitCSSViewportRule();

    std::string cssText() const final;
    void reattach(StyleRuleBase&) final;

    CSSStyleDeclaration& style();

private:
    WebKitCSSViewportRule(StyleRuleViewport&, CSSStyleSheet*);

    CSSRule::Type type() const final { return WEBKIT_VIEWPORT_RULE; }

    ref_ptr<StyleRuleViewport> m_viewportRule;
    mutable std::shared_ptr<StyleRuleCSSStyleDeclaration> m_propertiesCSSOMWrapper;
};

} // namespace WebCore

#endif // ENABLE(CSS_DEVICE_ADAPTATION)
