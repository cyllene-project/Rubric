/*
 * Copyright (C) 2011 Adobe Systems Incorporated. All rights reserved.
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

#include "WindRule.h"
#include <wtf/RefPtr.h>
#include <wtf/TypeCasts.h>
#include <vector>namespace WebCore {

class CSSPrimitiveValue;
class SVGPathByteStream;

class CSSBasicShape {
public:
    enum Type {
        CSSBasicShapePolygonType,
        CSSBasicShapeCircleType,
        CSSBasicShapeEllipseType,
        CSSBasicShapeInsetType,
        CSSBasicShapePathType
    };

    virtual Type type() const = 0;
    virtual String cssText() const = 0;
    virtual bool equals(const CSSBasicShape&) const = 0;

public:
    virtual ~CSSBasicShape() = default;

protected:
    CSSBasicShape() = default;
    std::shared_ptr<CSSPrimitiveValue> m_referenceBox;
};

class CSSBasicShapeInset final : public CSSBasicShape {
public:
    static ref_ptr<CSSBasicShapeInset> create() { return adoptRef(*new CSSBasicShapeInset); }

    CSSPrimitiveValue* top() const { return m_top.get(); }
    CSSPrimitiveValue* right() const { return m_right.get(); }
    CSSPrimitiveValue* bottom() const { return m_bottom.get(); }
    CSSPrimitiveValue* left() const { return m_left.get(); }

    CSSPrimitiveValue* topLeftRadius() const { return m_topLeftRadius.get(); }
    CSSPrimitiveValue* topRightRadius() const { return m_topRightRadius.get(); }
    CSSPrimitiveValue* bottomRightRadius() const { return m_bottomRightRadius.get(); }
    CSSPrimitiveValue* bottomLeftRadius() const { return m_bottomLeftRadius.get(); }

    void setTop(ref_ptr<CSSPrimitiveValue>&& top) { m_top = std::move(top); }
    void setRight(ref_ptr<CSSPrimitiveValue>&& right) { m_right = std::move(right); }
    void setBottom(ref_ptr<CSSPrimitiveValue>&& bottom) { m_bottom = std::move(bottom); }
    void setLeft(ref_ptr<CSSPrimitiveValue>&& left) { m_left = std::move(left); }

    void updateShapeSize4Values(ref_ptr<CSSPrimitiveValue>&& top, ref_ptr<CSSPrimitiveValue>&& right, ref_ptr<CSSPrimitiveValue>&& bottom, ref_ptr<CSSPrimitiveValue>&& left)
    {
        setTop(std::move(top));
        setRight(std::move(right));
        setBottom(std::move(bottom));
        setLeft(std::move(left));
    }

    void updateShapeSize1Value(ref_ptr<CSSPrimitiveValue>&& value1)
    {
        updateShapeSize4Values(value1.copyRef(), value1.copyRef(), value1.copyRef(), value1.copyRef());
    }

    void updateShapeSize2Values(ref_ptr<CSSPrimitiveValue>&& value1, ref_ptr<CSSPrimitiveValue>&& value2)
    {
        updateShapeSize4Values(value1.copyRef(), value2.copyRef(), value1.copyRef(), value2.copyRef());
    }

    void updateShapeSize3Values(ref_ptr<CSSPrimitiveValue>&& value1, ref_ptr<CSSPrimitiveValue>&& value2,  ref_ptr<CSSPrimitiveValue>&& value3)
    {
        updateShapeSize4Values(std::move(value1), value2.copyRef(), std::move(value3), value2.copyRef());
    }

    void setTopLeftRadius(std::shared_ptr<CSSPrimitiveValue>&& radius) { m_topLeftRadius = std::move(radius); }
    void setTopRightRadius(std::shared_ptr<CSSPrimitiveValue>&& radius) { m_topRightRadius = std::move(radius); }
    void setBottomRightRadius(std::shared_ptr<CSSPrimitiveValue>&& radius) { m_bottomRightRadius = std::move(radius); }
    void setBottomLeftRadius(std::shared_ptr<CSSPrimitiveValue>&& radius) { m_bottomLeftRadius = std::move(radius); }

private:
    CSSBasicShapeInset() = default;

    Type type() const final { return CSSBasicShapeInsetType; }
    std::string cssText() const final;
    bool equals(const CSSBasicShape&) const final;

    std::shared_ptr<CSSPrimitiveValue> m_top;
    std::shared_ptr<CSSPrimitiveValue> m_right;
    std::shared_ptr<CSSPrimitiveValue> m_bottom;
    std::shared_ptr<CSSPrimitiveValue> m_left;

    std::shared_ptr<CSSPrimitiveValue> m_topLeftRadius;
    std::shared_ptr<CSSPrimitiveValue> m_topRightRadius;
    std::shared_ptr<CSSPrimitiveValue> m_bottomRightRadius;
    std::shared_ptr<CSSPrimitiveValue> m_bottomLeftRadius;
};

class CSSBasicShapeCircle final : public CSSBasicShape {
public:
    static ref_ptr<CSSBasicShapeCircle> create() { return adoptRef(*new CSSBasicShapeCircle); }

    CSSPrimitiveValue* centerX() const { return m_centerX.get(); }
    CSSPrimitiveValue* centerY() const { return m_centerY.get(); }
    CSSPrimitiveValue* radius() const { return m_radius.get(); }

    void setCenterX(ref_ptr<CSSPrimitiveValue>&& centerX) { m_centerX = std::move(centerX); }
    void setCenterY(ref_ptr<CSSPrimitiveValue>&& centerY) { m_centerY = std::move(centerY); }
    void setRadius(ref_ptr<CSSPrimitiveValue>&& radius) { m_radius = std::move(radius); }

private:
    CSSBasicShapeCircle() = default;

    Type type() const final { return CSSBasicShapeCircleType; }
    std::string cssText() const final;
    bool equals(const CSSBasicShape&) const final;

    std::shared_ptr<CSSPrimitiveValue> m_centerX;
    std::shared_ptr<CSSPrimitiveValue> m_centerY;
    std::shared_ptr<CSSPrimitiveValue> m_radius;
};

class CSSBasicShapeEllipse final : public CSSBasicShape {
public:
    static ref_ptr<CSSBasicShapeEllipse> create() { return adoptRef(*new CSSBasicShapeEllipse); }

    CSSPrimitiveValue* centerX() const { return m_centerX.get(); }
    CSSPrimitiveValue* centerY() const { return m_centerY.get(); }
    CSSPrimitiveValue* radiusX() const { return m_radiusX.get(); }
    CSSPrimitiveValue* radiusY() const { return m_radiusY.get(); }

    void setCenterX(ref_ptr<CSSPrimitiveValue>&& centerX) { m_centerX = std::move(centerX); }
    void setCenterY(ref_ptr<CSSPrimitiveValue>&& centerY) { m_centerY = std::move(centerY); }
    void setRadiusX(ref_ptr<CSSPrimitiveValue>&& radiusX) { m_radiusX = std::move(radiusX); }
    void setRadiusY(ref_ptr<CSSPrimitiveValue>&& radiusY) { m_radiusY = std::move(radiusY); }

private:
    CSSBasicShapeEllipse() = default;

    Type type() const final { return CSSBasicShapeEllipseType; }
    std::string cssText() const final;
    bool equals(const CSSBasicShape&) const final;

    std::shared_ptr<CSSPrimitiveValue> m_centerX;
    std::shared_ptr<CSSPrimitiveValue> m_centerY;
    std::shared_ptr<CSSPrimitiveValue> m_radiusX;
    std::shared_ptr<CSSPrimitiveValue> m_radiusY;
};

class CSSBasicShapePolygon final : public CSSBasicShape {
public:
    static ref_ptr<CSSBasicShapePolygon> create() { return adoptRef(*new CSSBasicShapePolygon); }

    void appendPoint(ref_ptr<CSSPrimitiveValue>&& x, ref_ptr<CSSPrimitiveValue>&& y)
    {
        m_values.append(std::move(x));
        m_values.append(std::move(y));
    }

    const std::vector<ref_ptr<CSSPrimitiveValue>>& values() const { return m_values; }

    void setWindRule(WindRule rule) { m_windRule = rule; }
    WindRule windRule() const { return m_windRule; }

private:
    CSSBasicShapePolygon()
        : m_windRule(WindRule::NonZero)
    {
    }

    Type type() const final { return CSSBasicShapePolygonType; }
    std::string cssText() const final;
    bool equals(const CSSBasicShape&) const final;

    std::vector<ref_ptr<CSSPrimitiveValue>> m_values;
    WindRule m_windRule;
};

class CSSBasicShapePath final : public CSSBasicShape {
public:
    static ref_ptr<CSSBasicShapePath> create(std::unique_ptr<SVGPathByteStream>&& pathData)
    {
        return ref_ptr<CSSBasicShapePath>(std::move(pathData));
    }

    const SVGPathByteStream& pathData() const
    {
        return *m_byteStream;
    }

    void setWindRule(WindRule rule) { m_windRule = rule; }
    WindRule windRule() const { return m_windRule; }

private:
    CSSBasicShapePath(std::unique_ptr<SVGPathByteStream>&&);

    Type type() const final { return CSSBasicShapePathType; }
    std::string cssText() const final;
    bool equals(const CSSBasicShape&) const final;

    std::unique_ptr<SVGPathByteStream> m_byteStream;
    WindRule m_windRule { WindRule::NonZero };
};

} // namespace WebCore

#define SPECIALIZE_TYPE_TRAITS_CSS_BASIC_SHAPES(ToValueTypeName) \
SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::ToValueTypeName) \
    static bool isType(const WebCore::CSSBasicShape& basicShape) { return basicShape.type() == WebCore::CSSBasicShape::ToValueTypeName##Type; } \
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_CSS_BASIC_SHAPES(CSSBasicShapeInset)
SPECIALIZE_TYPE_TRAITS_CSS_BASIC_SHAPES(CSSBasicShapeCircle)
SPECIALIZE_TYPE_TRAITS_CSS_BASIC_SHAPES(CSSBasicShapeEllipse)
SPECIALIZE_TYPE_TRAITS_CSS_BASIC_SHAPES(CSSBasicShapePolygon)
SPECIALIZE_TYPE_TRAITS_CSS_BASIC_SHAPES(CSSBasicShapePath)
