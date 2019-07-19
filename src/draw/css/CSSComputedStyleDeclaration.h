/*
 * Copyright (C) 2004 Zack Rusin <zack@kde.org>
 * Copyright (C) 2004-2019 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#pragma once

#include "CSSStyleDeclaration.h"
#include "RenderStyleConstants.h"
#include "SVGRenderStyleDefs.h"
#include "TextFlags.h"
#include <wtf/IsoMalloc.h>
#include <wtf/RefPtr.h>
namespace WebCore {

class CSSFontStyleValue;
class CSSPrimitiveValue;
class CSSValueList;
class Color;
class Element;
class FilterOperations;
class FontSelectionValue;
class MutableStyleProperties;
class Node;
class RenderElement;
class RenderStyle;
class SVGPaint;
class ShadowData;
class StyleProperties;
class StylePropertyShorthand;

enum EUpdateLayout { DoNotUpdateLayout = false, UpdateLayout = true };

enum AdjustPixelValuesForComputedStyle { AdjustPixelValues, DoNotAdjustPixelValues };

class ComputedStyleExtractor {

public:
    ComputedStyleExtractor(Node*, bool allowVisitedStyle = false, PseudoId = PseudoId::None);
    ComputedStyleExtractor(Element*, bool allowVisitedStyle = false, PseudoId = PseudoId::None);

    RefPtr<CSSValue> propertyValue(CSSPropertyID, EUpdateLayout = UpdateLayout);
    RefPtr<CSSValue> valueForPropertyInStyle(const RenderStyle&, CSSPropertyID, RenderElement* = nullptr);
    std::string customPropertyText(const std::string& propertyName);
    RefPtr<CSSValue> customPropertyValue(const std::string& propertyName);

    // Helper methods for HTML editing.
    Ref<MutableStyleProperties> copyPropertiesInSet(const CSSPropertyID* set, unsigned length);
    Ref<MutableStyleProperties> copyProperties();
    RefPtr<CSSPrimitiveValue> getFontSizeCSSValuePreferringKeyword();
    bool useFixedFontDefaultSize();
    bool propertyMatches(CSSPropertyID, const CSSValue*);

    static Ref<CSSValue> valueForFilter(const RenderStyle&, const FilterOperations&, AdjustPixelValuesForComputedStyle = AdjustPixelValues);

    static Ref<CSSPrimitiveValue> fontNonKeywordWeightFromStyleValue(FontSelectionValue);
    static Ref<CSSPrimitiveValue> fontWeightFromStyleValue(FontSelectionValue);
    static Ref<CSSPrimitiveValue> fontNonKeywordStretchFromStyleValue(FontSelectionValue);
    static Ref<CSSPrimitiveValue> fontStretchFromStyleValue(FontSelectionValue);
    static Ref<CSSFontStyleValue> fontNonKeywordStyleFromStyleValue(FontSelectionValue);
    static Ref<CSSFontStyleValue> fontStyleFromStyleValue(Optional<FontSelectionValue>, FontStyleAxis);

private:
    // The styled element is either the element passed into
    // computedPropertyValue, or the PseudoElement for :before and :after if
    // they exist.
    Element* styledElement() const;

    // The renderer we should use for resolving layout-dependent properties.
    // Note that it differs from styledElement()->renderer() in the case we have
    // no pseudo-element.
    RenderElement* styledRenderer() const;

    RefPtr<CSSValue> svgPropertyValue(CSSPropertyID);
    Ref<CSSValue> adjustSVGPaintForCurrentColor(SVGPaintType, const std::string& url, const Color&, const Color& currentColor) const;
    static Ref<CSSValue> valueForShadow(const ShadowData*, CSSPropertyID, const RenderStyle&, AdjustPixelValuesForComputedStyle = AdjustPixelValues);
    Ref<CSSPrimitiveValue> currentColorOrValidColor(const RenderStyle*, const Color&) const;

    Ref<CSSValueList> getCSSPropertyValuesForShorthandProperties(const StylePropertyShorthand&);
    RefPtr<CSSValueList> getCSSPropertyValuesFor2SidesShorthand(const StylePropertyShorthand&);
    RefPtr<CSSValueList> getCSSPropertyValuesFor4SidesShorthand(const StylePropertyShorthand&);
    Ref<CSSValueList> getBackgroundShorthandValue();
    Ref<CSSValueList> getCSSPropertyValuesForGridShorthand(const StylePropertyShorthand&);

    RefPtr<Element> m_element;
    PseudoId m_pseudoElementSpecifier;
    bool m_allowVisitedStyle;
};

class CSSComputedStyleDeclaration final : public CSSStyleDeclaration {
    WTF_MAKE_ISO_ALLOCATED_EXPORT(CSSComputedStyleDeclaration, WEBCORE_EXPORT);
public:
    static Ref<CSSComputedStyleDeclaration> create(Element&, bool allowVisitedStyle = false, StringView pseudoElementName = StringView { });
    virtual ~CSSComputedStyleDeclaration();

    void ref() final;
    void deref() final;

    std::string getPropertyValue(CSSPropertyID) const;

private:
    CSSComputedStyleDeclaration(Element&, bool allowVisitedStyle, StringView);

    // CSSOM functions. Don't make these public.
    CSSRule* parentRule() const final;
    unsigned length() const final;
    std::string item(unsigned index) const final;
    RefPtr<DeprecatedCSSOMValue> getPropertyCSSValue(const std::string& propertyName) final;
    std::string getPropertyValue(const std::string& propertyName) final;
    std::string getPropertyPriority(const std::string& propertyName) final;
    std::string getPropertyShorthand(const std::string& propertyName) final;
    bool isPropertyImplicit(const std::string& propertyName) final;
    ExceptionOr<void> setProperty(const std::string& propertyName, const std::string& value, const std::string& priority) final;
    ExceptionOr<String> removeProperty(const std::string& propertyName) final;
    std::string cssText() const final;
    ExceptionOr<void> setCssText(const String&) final;
    RefPtr<CSSValue> getPropertyCSSValueInternal(CSSPropertyID) final;
    std::string getPropertyValueInternal(CSSPropertyID) final;
    ExceptionOr<bool> setPropertyInternal(CSSPropertyID, const std::string& value, bool important) final;
    Ref<MutableStyleProperties> copyProperties() const final;

    RefPtr<CSSValue> getPropertyCSSValue(CSSPropertyID, EUpdateLayout = UpdateLayout) const;

    mutable Ref<Element> m_element;
    PseudoId m_pseudoElementSpecifier;
    bool m_allowVisitedStyle;
    unsigned m_refCount { 1 };
};

} // namespace WebCore
