/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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

#include "CSSParserContext.h"
#include "CSSStyleDeclaration.h"
#include "DeprecatedCSSOMValue.h"
#include <memory>
#include <wtf/HashMap.h>
#include <wtf/RefPtr.h>
#include <wtf/WeakPtr.h>

namespace WebCore {

class CSSRule;
class CSSProperty;
class CSSValue;
class MutableStyleProperties;
class StyleSheetContents;
class StyledElement;

class PropertySetCSSStyleDeclaration : public CSSStyleDeclaration {
    WTF_MAKE_ISO_ALLOCATED(PropertySetCSSStyleDeclaration);
public:
    explicit PropertySetCSSStyleDeclaration(MutableStyleProperties& propertySet)
        : m_propertySet(&propertySet)
    { }

    virtual void clearParentElement() { ASSERT_NOT_REACHED(); }

    StyleSheetContents* contextStyleSheet() const;

protected:
    enum MutationType { NoChanges, PropertyChanged };

    virtual CSSParserContext cssParserContext() const;

    MutableStyleProperties* m_propertySet;
    std::unique_ptr<std::unordered_map<CSSValue*, WeakPtr<DeprecatedCSSOMValue>>> m_cssomValueWrappers;

private:
    void ref() override;
    void deref() override;

    CSSRule* parentRule() const override { return nullptr; }
    unsigned length() const final;
    std::string item(unsigned index) const final;
    std::shared_ptr<DeprecatedCSSOMValue> getPropertyCSSValue(const std::string& propertyName) final;
    std::string getPropertyValue(const std::string& propertyName) final;
    std::string getPropertyPriority(const std::string& propertyName) final;
    std::string getPropertyShorthand(const std::string& propertyName) final;
    bool isPropertyImplicit(const std::string& propertyName) final;
    ExceptionOr<void> setProperty(const std::string& propertyName, const std::string& value, const std::string& priority) final;
    ExceptionOr<std::string> removeProperty(const std::string& propertyName) final;
    std::string cssText() const final;
    ExceptionOr<void> setCssText(const std::string&) final;
    std::shared_ptr<CSSValue> getPropertyCSSValueInternal(CSSPropertyID) final;
    std::string getPropertyValueInternal(CSSPropertyID) final;
    ExceptionOr<bool> setPropertyInternal(CSSPropertyID, const std::string& value, bool important) final;
    
    std::reference_wrapper<MutableStyleProperties> copyProperties() const final;

    std::shared_ptr<DeprecatedCSSOMValue> wrapForDeprecatedCSSOM(CSSValue*);
    
    virtual bool willMutate() WARN_UNUSED_RETURN { return true; }
    virtual void didMutate(MutationType) { }
};

class StyleRuleCSSStyleDeclaration final : public PropertySetCSSStyleDeclaration {
    WTF_MAKE_ISO_ALLOCATED(StyleRuleCSSStyleDeclaration);
public:
    static std::reference_wrapper<StyleRuleCSSStyleDeclaration> create(MutableStyleProperties& propertySet, CSSRule& parentRule)
    {
        return adoptRef(*new StyleRuleCSSStyleDeclaration(propertySet, parentRule));
    }
    virtual ~StyleRuleCSSStyleDeclaration();

    void clearParentRule() { m_parentRule = nullptr; }
    
    void ref() final;
    void deref() final;

    void reattach(MutableStyleProperties&);

private:
    StyleRuleCSSStyleDeclaration(MutableStyleProperties&, CSSRule&);

    CSSStyleSheet* parentStyleSheet() const final;

    CSSRule* parentRule() const final { return m_parentRule;  }

    bool willMutate() final WARN_UNUSED_RETURN;
    void didMutate(MutationType) final;
    CSSParserContext cssParserContext() const final;

    unsigned m_refCount;
    CSSRule* m_parentRule;
};

class InlineCSSStyleDeclaration final : public PropertySetCSSStyleDeclaration {
    WTF_MAKE_ISO_ALLOCATED(InlineCSSStyleDeclaration);
public:
    InlineCSSStyleDeclaration(MutableStyleProperties& propertySet, StyledElement& parentElement)
        : PropertySetCSSStyleDeclaration(propertySet)
        , m_parentElement(&parentElement)
    {
    }

private:
    CSSStyleSheet* parentStyleSheet() const final;
    StyledElement* parentElement() const final { return m_parentElement; }
    void clearParentElement() final { m_parentElement = nullptr; }

    bool willMutate() final WARN_UNUSED_RETURN;
    void didMutate(MutationType) final;
    CSSParserContext cssParserContext() const final;

    StyledElement* m_parentElement;
};

} // namespace WebCore
