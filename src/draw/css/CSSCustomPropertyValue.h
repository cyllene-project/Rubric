/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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

#include "CSSRegisteredCustomProperty.h"
#include "CSSValue.h"
#include "CSSVariableReferenceValue.h"
//#include "Length.h"
//#include "StyleImage.h"

namespace WebCore {

class CSSParserToken;
class CSSVariableReferenceValue;
class RenderStyle;

class CSSCustomPropertyValue final : public CSSValue {
public:
    using VariantValue = Variant<std::reference_wrapper<CSSVariableReferenceValue>, CSSValueID, std::reference_wrapper<CSSVariableData>, Length, std::reference_wrapper<StyleImage>>;

    static std::reference_wrapper<CSSCustomPropertyValue> createUnresolved(const std::atomic<std::string>& name, std::reference_wrapper<CSSVariableReferenceValue>&& value)
    {
        return adoptRef(*new CSSCustomPropertyValue(name, { std::move(value) }));
    }

    static std::reference_wrapper<CSSCustomPropertyValue> createUnresolved(const std::atomic<std::string>& name, CSSValueID value)
    {
        return adoptRef(*new CSSCustomPropertyValue(name, { value }));
    }

    static std::reference_wrapper<CSSCustomPropertyValue> createWithID(const std::atomic<std::string>& name, CSSValueID id)
    {
        assert(id == CSSValueInherit || id == CSSValueInitial || id == CSSValueUnset || id == CSSValueRevert || id == CSSValueInvalid);
        return adoptRef(*new CSSCustomPropertyValue(name, { id }));
    }

    static std::reference_wrapper<CSSCustomPropertyValue> createSyntaxAll(const std::atomic<std::string>& name, std::reference_wrapper<CSSVariableData>&& value)
    {
        return adoptRef(*new CSSCustomPropertyValue(name, { std::move(value) }));
    }
    
    static std::reference_wrapper<CSSCustomPropertyValue> createSyntaxLength(const std::atomic<std::string>& name, Length value)
    {
        assert(!value.isUndefined());
        assert(!value.isCalculated());
        return adoptRef(*new CSSCustomPropertyValue(name, { std::move(value) }));
    }

    static std::reference_wrapper<CSSCustomPropertyValue> createSyntaxImage(const std::atomic<std::string>& name, std::reference_wrapper<StyleImage>&& value)
    {
        return adoptRef(*new CSSCustomPropertyValue(name, { std::move(value) }));
    }

    static std::reference_wrapper<CSSCustomPropertyValue> create(const CSSCustomPropertyValue& other)
    {
        return adoptRef(*new CSSCustomPropertyValue(other));
    }
    
    std::string customCSSText() const;

    const std::atomic<std::string>& name() const { return m_name; }
    bool isResolved() const  { return !WTF::holds_alternative<std::reference_wrapper<CSSVariableReferenceValue>>(m_value); }
    bool isUnset() const  { return WTF::holds_alternative<CSSValueID>(m_value) && WTF::get<CSSValueID>(m_value) == CSSValueUnset; }
    bool isInvalid() const  { return WTF::holds_alternative<CSSValueID>(m_value) && WTF::get<CSSValueID>(m_value) == CSSValueInvalid; }

    const VariantValue& value() const { return m_value; }

    std::vector<CSSParserToken> tokens() const;
    bool equals(const CSSCustomPropertyValue& other) const;

private:
    CSSCustomPropertyValue(const std::atomic<std::string>& name, VariantValue&& value)
        : CSSValue(CustomPropertyClass)
        , m_name(name)
        , m_value(std::move(value))
        , m_serialized(false)
    {
    }

    CSSCustomPropertyValue(const CSSCustomPropertyValue& other)
        : CSSValue(CustomPropertyClass)
        , m_name(other.m_name)
        , m_value(CSSValueUnset)
        , m_stringValue(other.m_stringValue)
        , m_serialized(other.m_serialized)
    {
        // No copy constructor for std::reference_wrapper<CSSVariableData>, so we have to do this ourselves
        auto visitor = WTF::makeVisitor([&](const std::reference_wrapper<CSSVariableReferenceValue>& value) {
            m_value = value.copyRef();
        }, [&](const CSSValueID& value) {
            m_value = value;
        }, [&](const std::reference_wrapper<CSSVariableData>& value) {
            m_value = value.copyRef();
        }, [&](const Length& value) {
            m_value = value;
        }, [&](const std::reference_wrapper<StyleImage>& value) {
            m_value = value.copyRef();
        });
        WTF::visit(visitor, other.m_value);
    }
    
    const AtomString m_name;
    VariantValue m_value;
    
    mutable String m_stringValue;
    mutable bool m_serialized { false };
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSCustomPropertyValue, isCustomPropertyValue())
