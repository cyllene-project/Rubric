/*
 * Copyright (C) 2015 Igalia S.L. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "CSSValue.h"
#include "CSSValuePool.h"


namespace WebCore {

class CSSContentDistributionValue final : public CSSValue {
public:
    static ref_ptr<CSSContentDistributionValue> create(CSSValueID distribution, CSSValueID position, CSSValueID overflow)
    {
        return ref_ptr<CSSContentDistributionValue>(distribution, position, overflow);
    }
    ~CSSContentDistributionValue();

    ref_ptr<CSSPrimitiveValue> distribution() const { return CSSValuePool::singleton().createIdentifierValue(m_distribution); }
    ref_ptr<CSSPrimitiveValue> position() const { return CSSValuePool::singleton().createIdentifierValue(m_position); }
    ref_ptr<CSSPrimitiveValue> overflow() const { return CSSValuePool::singleton().createIdentifierValue(m_overflow); }

    std::string customCSSText() const;

    bool equals(const CSSContentDistributionValue&) const;

private:
    CSSContentDistributionValue(CSSValueID distribution, CSSValueID position, CSSValueID overflow);

    CSSValueID m_distribution;
    CSSValueID m_position;
    CSSValueID m_overflow;
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSContentDistributionValue, isContentDistributionValue())
