/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
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

#include "CSSValue.h"

namespace WebCore {

class CSSPrimitiveValue;

class CSSReflectValue final : public CSSValue {
public:
    static std::reference_wrapper<CSSReflectValue> create(std::reference_wrapper<CSSPrimitiveValue>&& direction, std::reference_wrapper<CSSPrimitiveValue>&& offset, std::shared_ptr<CSSValue>&& mask)
    {
        return adoptRef(*new CSSReflectValue(std::move(direction), std::move(offset), std::move(mask)));
    }

    CSSPrimitiveValue& direction() { return m_direction.get(); }
    CSSPrimitiveValue& offset() { return m_offset.get(); }
    const CSSPrimitiveValue& direction() const { return m_direction.get(); }
    const CSSPrimitiveValue& offset() const { return m_offset.get(); }
    CSSValue* mask() const { return m_mask.get(); }

    std::string customCSSText() const;

    bool equals(const CSSReflectValue&) const;

private:
    CSSReflectValue(std::reference_wrapper<CSSPrimitiveValue>&& direction, std::reference_wrapper<CSSPrimitiveValue>&& offset, std::shared_ptr<CSSValue>&& mask)
        : CSSValue(ReflectClass)
        , m_direction(std::move(direction))
        , m_offset(std::move(offset))
        , m_mask(std::move(mask))
    {
    }

    std::reference_wrapper<CSSPrimitiveValue> m_direction;
    std::reference_wrapper<CSSPrimitiveValue> m_offset;
    std::shared_ptr<CSSValue> m_mask;
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSReflectValue, isReflectValue())
