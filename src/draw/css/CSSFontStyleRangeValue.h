/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
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

#include "CSSPrimitiveValue.h"
#include "CSSValue.h"
#include "CSSValueList.h"

namespace WebCore {

class CSSFontStyleRangeValue final : public CSSValue {
public:
    static ref_ptr<CSSFontStyleRangeValue> create(ref_ptr<CSSPrimitiveValue>&& fontStyleValue)
    {
        return ref_ptr<CSSFontStyleRangeValue>(std::move(fontStyleValue), nullptr);
    }
    static ref_ptr<CSSFontStyleRangeValue> create(ref_ptr<CSSPrimitiveValue>&& fontStyleValue, std::shared_ptr<CSSValueList>&& obliqueValues)
    {
        return ref_ptr<CSSFontStyleRangeValue>(std::move(fontStyleValue), std::move(obliqueValues));
    }

    std::string customCSSText() const;

    bool equals(const CSSFontStyleRangeValue&) const;

    ref_ptr<CSSPrimitiveValue> fontStyleValue;
    std::shared_ptr<CSSValueList> obliqueValues;

private:
    CSSFontStyleRangeValue(ref_ptr<CSSPrimitiveValue>&& fontStyleValue, std::shared_ptr<CSSValueList>&& obliqueValues)
        : CSSValue(FontStyleRangeClass)
        , fontStyleValue(std::move(fontStyleValue))
        , obliqueValues(std::move(obliqueValues))
    {
    }
};

}

SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSFontStyleRangeValue, isFontStyleRangeValue())
