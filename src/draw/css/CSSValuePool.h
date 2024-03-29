/*
 * Copyright (C) 2011, 2012 Apple Inc. All rights reserved.
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

#include "CSSInheritedValue.h"
#include "CSSInitialValue.h"
#include "CSSPrimitiveValue.h"
#include "CSSPropertyNames.h"
#include "CSSRevertValue.h"
#include "CSSUnsetValue.h"
#include "CSSValueKeywords.h"
//#include "ColorHash.h"
#include <utility>

namespace WebCore {

class CSSValueList;

enum class FromSystemFontID { No, Yes };

class CSSValuePool {

public:
    static CSSValuePool& singleton();

    std::shared_ptr<CSSValueList> createFontFaceValue(const std::atomic<std::string>&);
    ref_ptr<CSSPrimitiveValue> createFontFamilyValue(const std::string&, FromSystemFontID = FromSystemFontID::No);
    ref_ptr<CSSInheritedValue> createInheritedValue() { return m_inheritedValue.get(); }
    ref_ptr<CSSInitialValue> createImplicitInitialValue() { return m_implicitInitialValue.get(); }
    ref_ptr<CSSInitialValue> createExplicitInitialValue() { return m_explicitInitialValue.get(); }
    ref_ptr<CSSUnsetValue> createUnsetValue() { return m_unsetValue.get(); }
    ref_ptr<CSSRevertValue> createRevertValue() { return m_revertValue.get(); }
    ref_ptr<CSSPrimitiveValue> createIdentifierValue(CSSValueID identifier);
    ref_ptr<CSSPrimitiveValue> createIdentifierValue(CSSPropertyID identifier);
    ref_ptr<CSSPrimitiveValue> createColorValue(const Color&);
    ref_ptr<CSSPrimitiveValue> createValue(double value, CSSPrimitiveValue::UnitType);
    ref_ptr<CSSPrimitiveValue> createValue(const std::string& value, CSSPrimitiveValue::UnitType type) { return CSSPrimitiveValue::create(value, type); }
    ref_ptr<CSSPrimitiveValue> createValue(const Length& value, const RenderStyle& style) { return CSSPrimitiveValue::create(value, style); }
    ref_ptr<CSSPrimitiveValue> createValue(const LengthSize& value, const RenderStyle& style) { return CSSPrimitiveValue::create(value, style); }
    template<typename T> static ref_ptr<CSSPrimitiveValue> createValue(T&& value) { return CSSPrimitiveValue::create(std::forward<T>(value)); }

    void drain();

private:
    CSSValuePool();

    typedef std::unordered_map<Color, std::shared_ptr<CSSPrimitiveValue>> ColorValueCache;
    ColorValueCache m_colorValueCache;

    typedef std::unordered_map<AtomString, std::shared_ptr<CSSValueList>> FontFaceValueCache;
    FontFaceValueCache m_fontFaceValueCache;

    typedef std::unordered_map<std::pair<String, bool>, std::shared_ptr<CSSPrimitiveValue>> FontFamilyValueCache;
    FontFamilyValueCache m_fontFamilyValueCache;

    friend class WTF::NeverDestroyed<CSSValuePool>;

    LazyNeverDestroyed<CSSInheritedValue> m_inheritedValue;
    LazyNeverDestroyed<CSSInitialValue> m_implicitInitialValue;
    LazyNeverDestroyed<CSSInitialValue> m_explicitInitialValue;
    LazyNeverDestroyed<CSSUnsetValue> m_unsetValue;
    LazyNeverDestroyed<CSSRevertValue> m_revertValue;

    LazyNeverDestroyed<CSSPrimitiveValue> m_transparentColor;
    LazyNeverDestroyed<CSSPrimitiveValue> m_whiteColor;
    LazyNeverDestroyed<CSSPrimitiveValue> m_blackColor;

    static const int maximumCacheableIntegerValue = 255;

    LazyNeverDestroyed<CSSPrimitiveValue> m_pixelValues[maximumCacheableIntegerValue + 1];
    LazyNeverDestroyed<CSSPrimitiveValue> m_percentValues[maximumCacheableIntegerValue + 1];
    LazyNeverDestroyed<CSSPrimitiveValue> m_numberValues[maximumCacheableIntegerValue + 1];
    LazyNeverDestroyed<CSSPrimitiveValue> m_identifierValues[numCSSValueKeywords];
};

} // namespace WebCore
