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

#include "CSSImageGeneratorValue.h"
#include "CSSPrimitiveValue.h"
//#include "Gradient.h"
#include <vector>

namespace WebCore {

class FloatPoint;
class StyleResolver;

enum CSSGradientType {
    CSSDeprecatedLinearGradient,
    CSSDeprecatedRadialGradient,
    CSSPrefixedLinearGradient,
    CSSPrefixedRadialGradient,
    CSSLinearGradient,
    CSSRadialGradient,
    CSSConicGradient
};
enum CSSGradientRepeat { NonRepeating, Repeating };

struct CSSGradientColorStop {
    std::shared_ptr<CSSPrimitiveValue> m_position; // percentage or length
    std::shared_ptr<CSSPrimitiveValue> m_color;
    Color m_resolvedColor;
    bool m_colorIsDerivedFromElement = false;
    bool isMidpoint = false;
    bool operator==(const CSSGradientColorStop& other) const
    {
        return compareCSSValuePtr(m_color, other.m_color)
            && compareCSSValuePtr(m_position, other.m_position);
    }
};

class CSSGradientValue : public CSSImageGeneratorValue {
public:
    std::shared_ptr<Image> image(RenderElement&, const FloatSize&);

    void setFirstX(std::shared_ptr<CSSPrimitiveValue>&& val) { m_firstX = std::move(val); }
    void setFirstY(std::shared_ptr<CSSPrimitiveValue>&& val) { m_firstY = std::move(val); }
    void setSecondX(std::shared_ptr<CSSPrimitiveValue>&& val) { m_secondX = std::move(val); }
    void setSecondY(std::shared_ptr<CSSPrimitiveValue>&& val) { m_secondY = std::move(val); }

    void addStop(const CSSGradientColorStop& stop) { m_stops.append(stop); }
    void doneAddingStops() { m_stops.shrinkToFit(); }

    unsigned stopCount() const { return m_stops.size(); }

    void sortStopsIfNeeded();

    bool isRepeating() const { return m_repeating; }

    CSSGradientType gradientType() const { return m_gradientType; }

    bool isFixedSize() const { return false; }
    FloatSize fixedSize(const RenderElement&) const { return FloatSize(); }

    bool isPending() const { return false; }
    bool knownToBeOpaque(const RenderElement&) const;

    void loadSubimages(CachedResourceLoader&, const ResourceLoaderOptions&) { }
    std::reference_wrapper<CSSGradientValue> gradientWithStylesResolved(const StyleResolver&);

protected:
    CSSGradientValue(ClassType classType, CSSGradientRepeat repeat, CSSGradientType gradientType)
        : CSSImageGeneratorValue(classType)
        , m_stopsSorted(false)
        , m_gradientType(gradientType)
        , m_repeating(repeat == Repeating)
    {
    }

    CSSGradientValue(const CSSGradientValue& other, ClassType classType, CSSGradientType gradientType)
        : CSSImageGeneratorValue(classType)
        , m_firstX(other.m_firstX)
        , m_firstY(other.m_firstY)
        , m_secondX(other.m_secondX)
        , m_secondY(other.m_secondY)
        , m_stops(other.m_stops)
        , m_stopsSorted(other.m_stopsSorted)
        , m_gradientType(gradientType)
        , m_repeating(other.isRepeating() ? Repeating : NonRepeating)
    {
    }

    template<typename GradientAdapter>
    Gradient::ColorStopVector computeStops(GradientAdapter&, const CSSToLengthConversionData&, const RenderStyle&, float maxLengthForRepeat);

    // Resolve points/radii to front end values.
    FloatPoint computeEndPoint(CSSPrimitiveValue*, CSSPrimitiveValue*, const CSSToLengthConversionData&, const FloatSize&);

    bool isCacheable() const;

    // Points. Some of these may be null.
    std::shared_ptr<CSSPrimitiveValue> m_firstX;
    std::shared_ptr<CSSPrimitiveValue> m_firstY;

    std::shared_ptr<CSSPrimitiveValue> m_secondX;
    std::shared_ptr<CSSPrimitiveValue> m_secondY;

    // Stops
    std::vector<CSSGradientColorStop, 2> m_stops;
    bool m_stopsSorted;
    CSSGradientType m_gradientType;
    bool m_repeating;
};

class CSSLinearGradientValue final : public CSSGradientValue {
public:
    static std::reference_wrapper<CSSLinearGradientValue> create(CSSGradientRepeat repeat, CSSGradientType gradientType = CSSLinearGradient)
    {
        return adoptRef(*new CSSLinearGradientValue(repeat, gradientType));
    }

    void setAngle(std::reference_wrapper<CSSPrimitiveValue>&& val) { m_angle = std::move(val); }

    std::string customCSSText() const;

    // Create the gradient for a given size.
    std::reference_wrapper<Gradient> createGradient(RenderElement&, const FloatSize&);

    std::reference_wrapper<CSSLinearGradientValue> clone() const
    {
        return adoptRef(*new CSSLinearGradientValue(*this));
    }

    bool equals(const CSSLinearGradientValue&) const;

private:
    CSSLinearGradientValue(CSSGradientRepeat repeat, CSSGradientType gradientType = CSSLinearGradient)
        : CSSGradientValue(LinearGradientClass, repeat, gradientType)
    {
    }

    CSSLinearGradientValue(const CSSLinearGradientValue& other)
        : CSSGradientValue(other, LinearGradientClass, other.gradientType())
        , m_angle(other.m_angle)
    {
    }

    std::shared_ptr<CSSPrimitiveValue> m_angle; // may be null.
};

class CSSRadialGradientValue final : public CSSGradientValue {
public:
    static std::reference_wrapper<CSSRadialGradientValue> create(CSSGradientRepeat repeat, CSSGradientType gradientType = CSSRadialGradient)
    {
        return adoptRef(*new CSSRadialGradientValue(repeat, gradientType));
    }

    std::reference_wrapper<CSSRadialGradientValue> clone() const
    {
        return adoptRef(*new CSSRadialGradientValue(*this));
    }

    std::string customCSSText() const;

    void setFirstRadius(std::shared_ptr<CSSPrimitiveValue>&& val) { m_firstRadius = std::move(val); }
    void setSecondRadius(std::shared_ptr<CSSPrimitiveValue>&& val) { m_secondRadius = std::move(val); }

    void setShape(std::shared_ptr<CSSPrimitiveValue>&& val) { m_shape = std::move(val); }
    void setSizingBehavior(std::shared_ptr<CSSPrimitiveValue>&& val) { m_sizingBehavior = std::move(val); }

    void setEndHorizontalSize(std::shared_ptr<CSSPrimitiveValue>&& val) { m_endHorizontalSize = std::move(val); }
    void setEndVerticalSize(std::shared_ptr<CSSPrimitiveValue>&& val) { m_endVerticalSize = std::move(val); }

    // Create the gradient for a given size.
    std::reference_wrapper<Gradient> createGradient(RenderElement&, const FloatSize&);

    bool equals(const CSSRadialGradientValue&) const;

private:
    CSSRadialGradientValue(CSSGradientRepeat repeat, CSSGradientType gradientType = CSSRadialGradient)
        : CSSGradientValue(RadialGradientClass, repeat, gradientType)
    {
    }

    CSSRadialGradientValue(const CSSRadialGradientValue& other)
        : CSSGradientValue(other, RadialGradientClass, other.gradientType())
        , m_firstRadius(other.m_firstRadius)
        , m_secondRadius(other.m_secondRadius)
        , m_shape(other.m_shape)
        , m_sizingBehavior(other.m_sizingBehavior)
        , m_endHorizontalSize(other.m_endHorizontalSize)
        , m_endVerticalSize(other.m_endVerticalSize)
    {
    }

    // Resolve points/radii to front end values.
    float resolveRadius(CSSPrimitiveValue&, const CSSToLengthConversionData&, float* widthOrHeight = 0);

    // These may be null for non-deprecated gradients.
    std::shared_ptr<CSSPrimitiveValue> m_firstRadius;
    std::shared_ptr<CSSPrimitiveValue> m_secondRadius;

    // The below are only used for non-deprecated gradients. Any of them may be null.
    std::shared_ptr<CSSPrimitiveValue> m_shape;
    std::shared_ptr<CSSPrimitiveValue> m_sizingBehavior;

    std::shared_ptr<CSSPrimitiveValue> m_endHorizontalSize;
    std::shared_ptr<CSSPrimitiveValue> m_endVerticalSize;
};

class CSSConicGradientValue final : public CSSGradientValue {
public:
    static std::reference_wrapper<CSSConicGradientValue> create(CSSGradientRepeat repeat)
    {
        return adoptRef(*new CSSConicGradientValue(repeat));
    }

    std::reference_wrapper<CSSConicGradientValue> clone() const
    {
        return adoptRef(*new CSSConicGradientValue(*this));
    }

    std::string customCSSText() const;

    void setAngle(std::shared_ptr<CSSPrimitiveValue>&& val) { m_angle = std::move(val); }

    // Create the gradient for a given size.
    std::reference_wrapper<Gradient> createGradient(RenderElement&, const FloatSize&);

    bool equals(const CSSConicGradientValue&) const;

private:
    CSSConicGradientValue(CSSGradientRepeat repeat)
        : CSSGradientValue(ConicGradientClass, repeat, CSSConicGradient)
    {
    }

    CSSConicGradientValue(const CSSConicGradientValue& other)
        : CSSGradientValue(other, ConicGradientClass, other.gradientType())
        , m_angle(other.m_angle)
    {
    }

    std::shared_ptr<CSSPrimitiveValue> m_angle; // may be null.
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSGradientValue, isGradientValue())
SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSLinearGradientValue, isLinearGradientValue())
SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSRadialGradientValue, isRadialGradientValue())
SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSConicGradientValue, isConicGradientValue())
