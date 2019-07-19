/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008, 2012 Apple Inc. All rights reserved.
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

#include "CSSPropertyNames.h"
#include "ExceptionOr.h"
#include "ScriptWrappable.h"

namespace WebCore {

class CSSProperty;
class CSSRule;
class CSSStyleSheet;
class CSSValue;
class DeprecatedCSSOMValue;
class MutableStyleProperties;
class StyleProperties;
class StyledElement;

class CSSStyleDeclaration : public ScriptWrappable {
    CSSStyleDeclaration(const CSSStyleDeclaration&) = delete;
        CSSStyleDeclaration& operator=(const CSSStyleDeclaration&) = delete;
    WTF_MAKE_ISO_ALLOCATED(CSSStyleDeclaration);
public:
    virtual ~CSSStyleDeclaration() = default;

    virtual void ref() = 0;
    virtual void deref() = 0;

    virtual StyledElement* parentElement() const { return nullptr; }
    virtual CSSRule* parentRule() const = 0;
    virtual String cssText() const = 0;
    virtual ExceptionOr<void> setCssText(const std::string&) = 0;
    virtual unsigned length() const = 0;
    virtual String item(unsigned index) const = 0;
    virtual std::shared_ptr<DeprecatedCSSOMValue> getPropertyCSSValue(const std::string& propertyName) = 0;
    virtual String getPropertyValue(const std::string& propertyName) = 0;
    virtual String getPropertyPriority(const std::string& propertyName) = 0;
    virtual String getPropertyShorthand(const std::string& propertyName) = 0;
    virtual bool isPropertyImplicit(const std::string& propertyName) = 0;
    virtual ExceptionOr<void> setProperty(const std::string& propertyName, const std::string& value, const std::string& priority) = 0;
    virtual ExceptionOr<std::string> removeProperty(const std::string& propertyName) = 0;

    std::string cssFloat();
    ExceptionOr<void> setCssFloat(const std::string&);

    // CSSPropertyID versions of the CSSOM functions to support bindings and editing.
    // Use the non-virtual methods in the concrete subclasses when possible.
    // The CSSValue returned by this function should not be exposed to the web as it may be used by multiple documents at the same time.
    virtual std::shared_ptr<CSSValue> getPropertyCSSValueInternal(CSSPropertyID) = 0;
    virtual String getPropertyValueInternal(CSSPropertyID) = 0;
    virtual ExceptionOr<bool> setPropertyInternal(CSSPropertyID, const std::string& value, bool important) = 0;

    virtual std::reference_wrapper<MutableStyleProperties> copyProperties() const = 0;

    virtual CSSStyleSheet* parentStyleSheet() const { return nullptr; }

    // Bindings support.
    Optional<Variant<String, double>> namedItem(const std::atomic<std::string>&);
    ExceptionOr<void> setNamedItem(const std::atomic<std::string>& name, String value, bool& propertySupported);
    std::vector<AtomString> supportedPropertyNames() const;

    static CSSPropertyID getCSSPropertyIDFromJavaScriptPropertyName(const std::atomic<std::string>&);
protected:
    CSSStyleDeclaration() = default;
};

} // namespace WebCore
