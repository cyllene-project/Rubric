/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008, 2009 Apple Inc. All rights reserved.
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

#include "CSSValue.h"

namespace WebCore {

class CSSPrimitiveValue;

// Used for text-shadow and box-shadow
class CSSShadowValue final : public CSSValue {
public:
    static std::reference_wrapper<CSSShadowValue> create(std::shared_ptr<CSSPrimitiveValue>&& x,
        std::shared_ptr<CSSPrimitiveValue>&& y,
        std::shared_ptr<CSSPrimitiveValue>&& blur,
        std::shared_ptr<CSSPrimitiveValue>&& spread,
        std::shared_ptr<CSSPrimitiveValue>&& style,
        std::shared_ptr<CSSPrimitiveValue>&& color)
    {
        return adoptRef(*new CSSShadowValue(std::move(x), std::move(y), std::move(blur), std::move(spread), std::move(style), std::move(color)));
    }

    std::string customCSSText() const;

    bool equals(const CSSShadowValue&) const;

    std::shared_ptr<CSSPrimitiveValue> x;
    std::shared_ptr<CSSPrimitiveValue> y;
    std::shared_ptr<CSSPrimitiveValue> blur;
    std::shared_ptr<CSSPrimitiveValue> spread;
    std::shared_ptr<CSSPrimitiveValue> style;
    std::shared_ptr<CSSPrimitiveValue> color;

private:
    CSSShadowValue(std::shared_ptr<CSSPrimitiveValue>&& x,
        std::shared_ptr<CSSPrimitiveValue>&& y,
        std::shared_ptr<CSSPrimitiveValue>&& blur,
        std::shared_ptr<CSSPrimitiveValue>&& spread,
        std::shared_ptr<CSSPrimitiveValue>&& style,
        std::shared_ptr<CSSPrimitiveValue>&& color);
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSShadowValue, isShadowValue())
