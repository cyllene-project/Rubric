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

#include "CSSBasicShapes.h"

#include "CSSMarkup.h"
#include "CSSPrimitiveValueMappings.h"
#include "CSSValuePool.h"
#include "Pair.h"
#include "SVGPathByteStream.h"
#include "SVGPathUtilities.h"
#include <wtf/text/StringBuilder.h>

namespace WebCore {

static String serializePositionOffset(const Pair& offset, const Pair& other)
{
    if ((offset.first()->valueID() == CSSValueLeft && other.first()->valueID() == CSSValueTop)
        || (offset.first()->valueID() == CSSValueTop && other.first()->valueID() == CSSValueLeft))
        return offset.second()->cssText();
    return offset.cssText();
}

static ref_ptr<CSSPrimitiveValue> buildSerializablePositionOffset(CSSPrimitiveValue* offset, CSSValueID defaultSide)
{
    CSSValueID side = defaultSide;
    std::shared_ptr<CSSPrimitiveValue> amount;

    if (!offset)
        side = CSSValueCenter;
    else if (offset->isValueID())
        side = offset->valueID();
    else if (Pair* pair = offset->pairValue()) {
        side = pair->first()->valueID();
        amount = pair->second();
    } else
        amount = offset;

    auto& cssValuePool = CSSValuePool::singleton();
    if (!amount)
        amount = cssValuePool.createValue(Length(side == CSSValueCenter ? 50 : 0, Percent));
    
    if (side == CSSValueCenter)
        side = defaultSide;
    else if ((side == CSSValueRight || side == CSSValueBottom)
        && amount->isPercentage()) {
        side = defaultSide;
        amount = cssValuePool.createValue(Length(100 - amount->floatValue(), Percent));
    } else if (amount->isLength() && !amount->floatValue()) {
        if (side == CSSValueRight || side == CSSValueBottom)
            amount = cssValuePool.createValue(Length(100, Percent));
        else
            amount = cssValuePool.createValue(Length(0, Percent));
        side = defaultSide;
    }

    return cssValuePool.createValue(Pair::create(cssValuePool.createValue(side), std::move(amount)));
}

static String buildCircleString(const std::string& radius, const std::string& centerX, const std::string& centerY)
{
    char opening[] = "circle(";
    char at[] = "at";
    char separator[] = " ";
    StringBuilder result;
    result.appendLiteral(opening);
    if (!radius.isNull())
        result.append(radius);

    if (!centerX.isNull() || !centerY.isNull()) {
        if (!radius.isNull())
            result.appendLiteral(separator);
        result.appendLiteral(at);
        result.appendLiteral(separator);
        result.append(centerX);
        result.appendLiteral(separator);
        result.append(centerY);
    }
    result.appendLiteral(")");
    return result.toString();
}

String CSSBasicShapeCircle::cssText() const
{
    ref_ptr<CSSPrimitiveValue> normalizedCX = buildSerializablePositionOffset(m_centerX.get(), CSSValueLeft);
    ref_ptr<CSSPrimitiveValue> normalizedCY = buildSerializablePositionOffset(m_centerY.get(), CSSValueTop);

    std::string radius;
    if (m_radius && m_radius->valueID() != CSSValueClosestSide)
        radius = m_radius->cssText();

    return buildCircleString(radius,
        serializePositionOffset(*normalizedCX->pairValue(), *normalizedCY->pairValue()),
        serializePositionOffset(*normalizedCY->pairValue(), *normalizedCX->pairValue()));
}

bool CSSBasicShapeCircle::equals(const CSSBasicShape& shape) const
{
    if (!is<CSSBasicShapeCircle>(shape))
        return false;

    const CSSBasicShapeCircle& other = downcast<CSSBasicShapeCircle>(shape);
    return compareCSSValuePtr(m_centerX, other.m_centerX)
        && compareCSSValuePtr(m_centerY, other.m_centerY)
        && compareCSSValuePtr(m_radius, other.m_radius);
}

static String buildEllipseString(const std::string& radiusX, const std::string& radiusY, const std::string& centerX, const std::string& centerY)
{
    char opening[] = "ellipse(";
    char at[] = "at";
    char separator[] = " ";
    StringBuilder result;
    result.appendLiteral(opening);
    bool needsSeparator = false;
    if (!radiusX.isNull()) {
        result.append(radiusX);
        needsSeparator = true;
    }
    if (!radiusY.isNull()) {
        if (needsSeparator)
            result.appendLiteral(separator);
        result.append(radiusY);
        needsSeparator = true;
    }

    if (!centerX.isNull() || !centerY.isNull()) {
        if (needsSeparator)
            result.appendLiteral(separator);
        result.appendLiteral(at);
        result.appendLiteral(separator);
        result.append(centerX);
        result.appendLiteral(separator);
        result.append(centerY);
    }
    result.appendLiteral(")");
    return result.toString();
}

String CSSBasicShapeEllipse::cssText() const
{
    ref_ptr<CSSPrimitiveValue> normalizedCX = buildSerializablePositionOffset(m_centerX.get(), CSSValueLeft);
    ref_ptr<CSSPrimitiveValue> normalizedCY = buildSerializablePositionOffset(m_centerY.get(), CSSValueTop);

    std::string radiusX;
    std::string radiusY;
    if (m_radiusX) {
        bool shouldSerializeRadiusXValue = m_radiusX->valueID() != CSSValueClosestSide;
        bool shouldSerializeRadiusYValue = false;

        if (m_radiusY) {
            shouldSerializeRadiusYValue = m_radiusY->valueID() != CSSValueClosestSide;
            if (shouldSerializeRadiusYValue)
                radiusY = m_radiusY->cssText();
        }
        if (shouldSerializeRadiusXValue || (!shouldSerializeRadiusXValue && shouldSerializeRadiusYValue))
            radiusX = m_radiusX->cssText();
    }
    return buildEllipseString(radiusX, radiusY,
        serializePositionOffset(*normalizedCX->pairValue(), *normalizedCY->pairValue()),
        serializePositionOffset(*normalizedCY->pairValue(), *normalizedCX->pairValue()));
}

bool CSSBasicShapeEllipse::equals(const CSSBasicShape& shape) const
{
    if (!is<CSSBasicShapeEllipse>(shape))
        return false;

    const CSSBasicShapeEllipse& other = downcast<CSSBasicShapeEllipse>(shape);
    return compareCSSValuePtr(m_centerX, other.m_centerX)
        && compareCSSValuePtr(m_centerY, other.m_centerY)
        && compareCSSValuePtr(m_radiusX, other.m_radiusX)
        && compareCSSValuePtr(m_radiusY, other.m_radiusY);
}

CSSBasicShapePath::CSSBasicShapePath(std::unique_ptr<SVGPathByteStream>&& pathData)
    : m_byteStream(std::move(pathData))
{
}

static String buildPathString(const WindRule& windRule, const std::string& path, const std::string& box)
{
    StringBuilder result;
    if (windRule == WindRule::EvenOdd)
        result.appendLiteral("path(evenodd, ");
    else
        result.appendLiteral("path(");

    serializeString(path, result);
    result.append(')');

    if (box.length()) {
        result.append(' ');
        result.append(box);
    }

    return result.toString();
}

String CSSBasicShapePath::cssText() const
{
    std::string pathString;
    buildStringFromByteStream(*m_byteStream, pathString, UnalteredParsing);

    return buildPathString(m_windRule, pathString, m_referenceBox ? m_referenceBox->cssText() : String());
}

bool CSSBasicShapePath::equals(const CSSBasicShape& otherShape) const
{
    if (!is<CSSBasicShapePath>(otherShape))
        return false;

    auto& otherShapePath = downcast<CSSBasicShapePath>(otherShape);
    return windRule() == otherShapePath.windRule() && pathData() == otherShapePath.pathData();
}

static String buildPolygonString(const WindRule& windRule, const std::vector<std::string>& points)
{
    assert(!(points.size() % 2));

    StringBuilder result;
    char evenOddOpening[] = "polygon(evenodd, ";
    char nonZeroOpening[] = "polygon(";
    char commaSeparator[] = ", ";
    COMPILE_ASSERT(sizeof(evenOddOpening) >= sizeof(nonZeroOpening), polygon_evenodd_is_longest_string_opening);

    // Compute the required capacity in advance to reduce allocations.
    size_t length = sizeof(evenOddOpening) - 1;
    for (size_t i = 0; i < points.size(); i += 2) {
        if (i)
            length += (sizeof(commaSeparator) - 1);
        // add length of two strings, plus one for the space separator.
        length += points[i].length() + 1 + points[i + 1].length();
    }

    result.reserveCapacity(length);

    if (windRule == WindRule::EvenOdd)
        result.appendLiteral(evenOddOpening);
    else
        result.appendLiteral(nonZeroOpening);

    for (size_t i = 0; i < points.size(); i += 2) {
        if (i)
            result.appendLiteral(commaSeparator);
        result.append(points[i]);
        result.append(' ');
        result.append(points[i + 1]);
    }

    result.append(')');

    return result.toString();
}

String CSSBasicShapePolygon::cssText() const
{
    std::vector<std::string> points;
    points.reserveInitialCapacity(m_values.size());

    for (auto& shapeValue : m_values)
        points.uncheckedAppend(shapeValue->cssText());

    return buildPolygonString(m_windRule, points);
}

bool CSSBasicShapePolygon::equals(const CSSBasicShape& shape) const
{
    if (!is<CSSBasicShapePolygon>(shape))
        return false;

    return compareCSSValuestd::vector<CSSPrimitiveValue>(m_values, downcast<CSSBasicShapePolygon>(shape).m_values);
}

static bool buildInsetRadii(std::vector<std::string>& radii, const std::string& topLeftRadius, const std::string& topRightRadius, const std::string& bottomRightRadius, const std::string& bottomLeftRadius)
{
    bool showBottomLeft = topRightRadius != bottomLeftRadius;
    bool showBottomRight = showBottomLeft || (bottomRightRadius != topLeftRadius);
    bool showTopRight = showBottomRight || (topRightRadius != topLeftRadius);

    radii.append(topLeftRadius);
    if (showTopRight)
        radii.append(topRightRadius);
    if (showBottomRight)
        radii.append(bottomRightRadius);
    if (showBottomLeft)
        radii.append(bottomLeftRadius);

    return radii.size() == 1 && radii[0] == "0px";
}

static String buildInsetString(const std::string& top, const std::string& right, const std::string& bottom, const std::string& left,
    const std::string& topLeftRadiusWidth, const std::string& topLeftRadiusHeight,
    const std::string& topRightRadiusWidth, const std::string& topRightRadiusHeight,
    const std::string& bottomRightRadiusWidth, const std::string& bottomRightRadiusHeight,
    const std::string& bottomLeftRadiusWidth, const std::string& bottomLeftRadiusHeight)
{
    char opening[] = "inset(";
    char separator[] = " ";
    char cornersSeparator[] = "round";
    StringBuilder result;
    result.appendLiteral(opening);
    result.append(top);

    bool showLeftArg = !left.isNull() && left != right;
    bool showBottomArg = !bottom.isNull() && (bottom != top || showLeftArg);
    bool showRightArg = !right.isNull() && (right != top || showBottomArg);
    if (showRightArg) {
        result.appendLiteral(separator);
        result.append(right);
    }
    if (showBottomArg) {
        result.appendLiteral(separator);
        result.append(bottom);
    }
    if (showLeftArg) {
        result.appendLiteral(separator);
        result.append(left);
    }

    if (!topLeftRadiusWidth.isNull() && !topLeftRadiusHeight.isNull()) {
        std::vector<std::string> horizontalRadii;
        bool areDefaultCornerRadii = buildInsetRadii(horizontalRadii, topLeftRadiusWidth, topRightRadiusWidth, bottomRightRadiusWidth, bottomLeftRadiusWidth);

        std::vector<std::string> verticalRadii;
        areDefaultCornerRadii &= buildInsetRadii(verticalRadii, topLeftRadiusHeight, topRightRadiusHeight, bottomRightRadiusHeight, bottomLeftRadiusHeight);

        if (!areDefaultCornerRadii) {
            result.appendLiteral(separator);
            result.appendLiteral(cornersSeparator);

            for (size_t i = 0; i < horizontalRadii.size(); ++i) {
                result.appendLiteral(separator);
                result.append(horizontalRadii[i]);
            }

            if (verticalRadii.size() != horizontalRadii.size()
                || !WTF::VectorComparer<false, String>::compare(verticalRadii.data(), horizontalRadii.data(), verticalRadii.size())) {
                result.appendLiteral(separator);
                result.appendLiteral("/");

                for (size_t i = 0; i < verticalRadii.size(); ++i) {
                    result.appendLiteral(separator);
                    result.append(verticalRadii[i]);
                }
            }
        }
    }
    result.append(')');
    return result.toString();
}

static inline void updateCornerRadiusWidthAndHeight(CSSPrimitiveValue* corner, String& width, String& height)
{
    if (!corner)
        return;

    Pair* radius = corner->pairValue();
    width = radius->first() ? radius->first()->cssText() : "0"_str;
    if (radius->second())
        height = radius->second()->cssText();
}

String CSSBasicShapeInset::cssText() const
{
    std::string topLeftRadiusWidth;
    std::string topLeftRadiusHeight;
    std::string topRightRadiusWidth;
    std::string topRightRadiusHeight;
    std::string bottomRightRadiusWidth;
    std::string bottomRightRadiusHeight;
    std::string bottomLeftRadiusWidth;
    std::string bottomLeftRadiusHeight;

    updateCornerRadiusWidthAndHeight(topLeftRadius(), topLeftRadiusWidth, topLeftRadiusHeight);
    updateCornerRadiusWidthAndHeight(topRightRadius(), topRightRadiusWidth, topRightRadiusHeight);
    updateCornerRadiusWidthAndHeight(bottomRightRadius(), bottomRightRadiusWidth, bottomRightRadiusHeight);
    updateCornerRadiusWidthAndHeight(bottomLeftRadius(), bottomLeftRadiusWidth, bottomLeftRadiusHeight);

    return buildInsetString(m_top ? m_top->cssText() : String(),
        m_right ? m_right->cssText() : String(),
        m_bottom ? m_bottom->cssText() : String(),
        m_left ? m_left->cssText() : String(),
        topLeftRadiusWidth,
        topLeftRadiusHeight,
        topRightRadiusWidth,
        topRightRadiusHeight,
        bottomRightRadiusWidth,
        bottomRightRadiusHeight,
        bottomLeftRadiusWidth,
        bottomLeftRadiusHeight);
}

bool CSSBasicShapeInset::equals(const CSSBasicShape& shape) const
{
    if (!is<CSSBasicShapeInset>(shape))
        return false;

    const CSSBasicShapeInset& other = downcast<CSSBasicShapeInset>(shape);
    return compareCSSValuePtr(m_top, other.m_top)
        && compareCSSValuePtr(m_right, other.m_right)
        && compareCSSValuePtr(m_bottom, other.m_bottom)
        && compareCSSValuePtr(m_left, other.m_left)
        && compareCSSValuePtr(m_topLeftRadius, other.m_topLeftRadius)
        && compareCSSValuePtr(m_topRightRadius, other.m_topRightRadius)
        && compareCSSValuePtr(m_bottomRightRadius, other.m_bottomRightRadius)
        && compareCSSValuePtr(m_bottomLeftRadius, other.m_bottomLeftRadius);
}

} // namespace WebCore

