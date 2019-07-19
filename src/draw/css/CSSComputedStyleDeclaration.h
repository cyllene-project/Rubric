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

    std::shared_ptr<CSSValue> propertyValue(CSSPropertyID, EUpdateLayout = UpdateLayout);
    std::shared_ptr<CSSValue> valueForPropertyInStyle(const RenderStyle&, CSSPropertyID, RenderElement* = nullptr);
    std::string customPropertyText(const std::string& propertyName);
    std::shared_ptr<CSSValue> customPropertyValue(const std::string& propertyName);

    // Helper methods for HTML editing.
    ref_ptr<MutableStyleProperties> copyPropertiesInSet(const CSSPropertyID* set, unsigned length);
    ref_ptr<MutableStyleProperties> copyProperties();
    std::shared_ptr<CSSPrimitiveValue> getFontSizeCSSValuePreferringKeyword();
    bool useFixedFontDefaultSize();
    bool propertyMatches(CSSPropertyID, const CSSValue*);

    static ref_ptr<CSSValue> valueForFilter(const RenderStyle&, const FilterOperations&, AdjustPixelValuesForComputedStyle = AdjustPixelValues);

    static ref_ptr<CSSPrimitiveValue> fontNonKeywordWeightFromStyleValue(FontSelectionValue);
    static ref_ptr<CSSPrimitiveValue> fontWeightFromStyleValue(FontSelectionValue);
    static ref_ptr<CSSPrimitiveValue> fontNonKeywordStretchFromStyleValue(FontSelectionValue);
    static ref_ptr<CSSPrimitiveValue> fontStretchFromStyleValue(FontSelectionValue);
    static ref_ptr<CSSFontStyleValue> fontNonKeywordStyleFromStyleValue(FontSelectionValue);
    static ref_ptr<CSSFontStyleValue> fontStyleFromStyleValue(std::optional<FontSelectionValue>, FontStyleAxis);

private:
    // The styled element is either the element passed into
    // computedPropertyValue, or the PseudoElement for :before and :after if
    // they exist.
    Element* styledElement() const;

    // The renderer we should use for resolving layout-dependent properties.
    // Note that it differs from styledElement()->renderer() in the case we have
    // no pseudo-element.
    RenderElement* styledRenderer() const;

    std::shared_ptr<CSSValue> svgPropertyValue(CSSPropertyID);
    ref_ptr<CSSValue> adjustSVGPaintForCurrentColor(SVGPaintType, const std::string& url, const Color&, const Color& currentColor) const;
    static ref_ptr<CSSValue> valueForShadow(const ShadowData*, CSSPropertyID, const RenderStyle&, AdjustPixelValuesForComputedStyle = AdjustPixelValues);
    ref_ptr<CSSPrimitiveValue> currentColorOrValidColor(const RenderStyle*, const Color&) const;

    ref_ptr<CSSValueList> getCSSPropertyValuesForShorthandProperties(const StylePropertyShorthand&);
    std::shared_ptr<CSSValueList> getCSSPropertyValuesFor2SidesShorthand(const StylePropertyShorthand&);
    std::shared_ptr<CSSValueList> getCSSPropertyValuesFor4SidesShorthand(const StylePropertyShorthand&);
    ref_ptr<CSSValueList> getBackgroundShorthandValue();
    ref_ptr<CSSValueList> getCSSPropertyValuesForGridShorthand(const StylePropertyShorthand&);

    std::shared_ptr<Element> m_element;
    PseudoId m_pseudoElementSpecifier;
    bool m_allowVisitedStyle;
};

class CSSComputedStyleDeclaration final : public CSSStyleDeclaration {
    WTF_MAKE_ISO_ALLOCATED_EXPORT(CSSComputedStyleDeclaration, WEBCORE_EXPORT);
public:
    static ref_ptr<CSSComputedStyleDeclaration> create(Element&, bool allowVisitedStyle = false, StringView pseudoElementName = StringView { });
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
    std::shared_ptr<DeprecatedCSSOMValue> getPropertyCSSValue(const std::string& propertyName) final;
    std::string getPropertyValue(const std::string& propertyName) final;
    std::string getPropertyPriority(const std::string& propertyName) final;
    std::string getPropertyShorthand(const std::string& propertyName) final;
    bool isPropertyImplicit(const std::string& propertyName) final;
    ExceptionOr<void> setProperty(const std::string& propertyName, const std::string& value, const std::string& priority) final;
    ExceptionOr<std::string> removeProperty(const std::string& propertyName) final;
    std::string cssText() const final;
    ExceptionOr<void> setCssText(const std::string&) final;
    std::shared_ptr<CSSValue> getPropertyCSSValueInternal(CSSPropertyID) final;
    std::string getPropertyValueInternal(CSSPropertyID) final;
    ExceptionOr<bool> setPropertyInternal(CSSPropertyID, const std::string& value, bool important) final;
    ref_ptr<MutableStyleProperties> copyProperties() const final;

    std::shared_ptr<CSSValue> getPropertyCSSValue(CSSPropertyID, EUpdateLayout = UpdateLayout) const;

    mutable ref_ptr<Element> m_element;
    PseudoId m_pseudoElementSpecifier;
    bool m_allowVisitedStyle;
    unsigned m_refCount { 1 };
};

} // namespace WebCore
