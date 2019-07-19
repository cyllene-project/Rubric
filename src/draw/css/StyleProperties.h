/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008, 2012, 2013 Apple Inc. All rights reserved.
 * Copyright (C) 2013 Intel Corporation. All rights reserved.
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

#include "CSSParserContext.h"
#include "CSSParserTokenRange.h"
#include "CSSProperty.h"
#include "CSSValueKeywords.h"
#include <memory>
#include <vector>

namespace WebCore {

class CSSDeferredParser;
class CSSStyleDeclaration;
class CachedResource;
class Color;
class ImmutableStyleProperties;
class MutableStyleProperties;
class PropertySetCSSStyleDeclaration;
class StyledElement;
class StylePropertyShorthand;
class StyleSheetContents;

enum StylePropertiesType { ImmutablePropertiesType, MutablePropertiesType, DeferredPropertiesType };
    
class StylePropertiesBase : public RefCounted<StylePropertiesBase> {
public:
    // Override RefCounted's deref() to ensure operator delete is called on
    // the appropriate subclass type.
    void deref();
    
    StylePropertiesType type() const { return static_cast<StylePropertiesType>(m_type); }

    CSSParserMode cssParserMode() const { return static_cast<CSSParserMode>(m_cssParserMode); }

protected:
    StylePropertiesBase(CSSParserMode cssParserMode, StylePropertiesType type)
        : m_cssParserMode(cssParserMode)
        , m_type(type)
        , m_arraySize(0)
    { }
    
    StylePropertiesBase(CSSParserMode cssParserMode, unsigned immutableArraySize)
        : m_cssParserMode(cssParserMode)
        , m_type(ImmutablePropertiesType)
        , m_arraySize(immutableArraySize)
    { }
    
    unsigned m_cssParserMode : 3;
    mutable unsigned m_type : 2;
    unsigned m_arraySize : 27;
};

class StyleProperties : public StylePropertiesBase {
    friend class PropertyReference;
public:
    class PropertyReference {
    public:
        PropertyReference(const StylePropertyMetadata& metadata, const CSSValue* value)
            : m_metadata(metadata)
            , m_value(value)
        { }

        CSSPropertyID id() const { return static_cast<CSSPropertyID>(m_metadata.m_propertyID); }
        CSSPropertyID shorthandID() const { return m_metadata.shorthandID(); }

        bool isImportant() const { return m_metadata.m_important; }
        bool isInherited() const { return m_metadata.m_inherited; }
        bool isImplicit() const { return m_metadata.m_implicit; }

        std::string cssName() const;
        std::string cssText() const;

        const CSSValue* value() const { return m_value; }
        // FIXME: We should try to remove this mutable overload.
        CSSValue* value() { return const_cast<CSSValue*>(m_value); }

        // FIXME: Remove this.
        CSSProperty toCSSProperty() const { return CSSProperty(id(), const_cast<CSSValue*>(m_value), isImportant(), m_metadata.m_isSetFromShorthand, m_metadata.m_indexInShorthandsVector, isImplicit()); }

    private:
        const StylePropertyMetadata& m_metadata;
        const CSSValue* m_value;
    };

    unsigned propertyCount() const;
    bool isEmpty() const { return !propertyCount(); }
    PropertyReference propertyAt(unsigned) const;

    std::shared_ptr<CSSValue> getPropertyCSSValue(CSSPropertyID) const;
    std::string getPropertyValue(CSSPropertyID) const;

    Optional<Color> propertyAsColor(CSSPropertyID) const;
    CSSValueID propertyAsValueID(CSSPropertyID) const;

    bool propertyIsImportant(CSSPropertyID) const;
    std::string getPropertyShorthand(CSSPropertyID) const;
    bool isPropertyImplicit(CSSPropertyID) const;

    std::shared_ptr<CSSValue> getCustomPropertyCSSValue(const std::string& propertyName) const;
    std::string getCustomPropertyValue(const std::string& propertyName) const;
    bool customPropertyIsImportant(const std::string& propertyName) const;

    std::reference_wrapper<MutableStyleProperties> copyBlockProperties() const;

    std::reference_wrapper<MutableStyleProperties> mutableCopy() const;
    std::reference_wrapper<ImmutableStyleProperties> immutableCopyIfNeeded() const;

    std::reference_wrapper<MutableStyleProperties> copyPropertiesInSet(const CSSPropertyID* set, unsigned length) const;
    
    std::string asText() const;

    bool hasCSSOMWrapper() const;
    bool isMutable() const { return type() == MutablePropertiesType; }

    bool traverseSubresources(const WTF::Function<bool (const CachedResource&)>& handler) const;

    static unsigned averageSizeInBytes();

#ifndef NDEBUG
    void showStyle();
#endif

    bool propertyMatches(CSSPropertyID, const CSSValue*) const;

protected:
    StyleProperties(CSSParserMode cssParserMode, StylePropertiesType type)
        : StylePropertiesBase(cssParserMode, type)
    { }

    StyleProperties(CSSParserMode cssParserMode, unsigned immutableArraySize)
        : StylePropertiesBase(cssParserMode, immutableArraySize)
    { }

    int findPropertyIndex(CSSPropertyID) const;
    int findCustomPropertyIndex(const std::string& propertyName) const;

private:
    std::string getShorthandValue(const StylePropertyShorthand&) const;
    std::string getCommonValue(const StylePropertyShorthand&) const;
    std::string getAlignmentShorthandValue(const StylePropertyShorthand&) const;
    std::string borderPropertyValue(const StylePropertyShorthand&, const StylePropertyShorthand&, const StylePropertyShorthand&) const;
    std::string pageBreakPropertyValue(const StylePropertyShorthand&) const;
    std::string getLayeredShorthandValue(const StylePropertyShorthand&) const;
    std::string get2Values(const StylePropertyShorthand&) const;
    std::string get4Values(const StylePropertyShorthand&) const;
    std::string borderSpacingValue(const StylePropertyShorthand&) const;
    std::string fontValue() const;
    void appendFontLonghandValueIfExplicit(CSSPropertyID, StringBuilder& result, String& value) const;
    
    std::shared_ptr<CSSValue> getPropertyCSSValueInternal(CSSPropertyID) const;
    
    friend class PropertySetCSSStyleDeclaration;
};

class ImmutableStyleProperties final : public StyleProperties {
public:
    ~ImmutableStyleProperties();
    static std::reference_wrapper<ImmutableStyleProperties> create(const CSSProperty* properties, unsigned count, CSSParserMode);

    unsigned propertyCount() const { return m_arraySize; }
    bool isEmpty() const { return !propertyCount(); }
    PropertyReference propertyAt(unsigned index) const;

    const CSSValue** valueArray() const;
    const StylePropertyMetadata* metadataArray() const;
    int findPropertyIndex(CSSPropertyID) const;
    int findCustomPropertyIndex(const std::string& propertyName) const;
    
    void* m_storage;

private:
    ImmutableStyleProperties(const CSSProperty*, unsigned count, CSSParserMode);
};

inline const CSSValue** ImmutableStyleProperties::valueArray() const
{
    return reinterpret_cast<const CSSValue**>(const_cast<const void**>((&(this->m_storage))));
}

inline const StylePropertyMetadata* ImmutableStyleProperties::metadataArray() const
{
    return reinterpret_cast_ptr<const StylePropertyMetadata*>(&reinterpret_cast_ptr<const char*>(&(this->m_storage))[m_arraySize * sizeof(CSSValue*)]);
}

class MutableStyleProperties final : public StyleProperties {
public:
    static std::reference_wrapper<MutableStyleProperties> create(CSSParserMode = HTMLQuirksMode);
    static std::reference_wrapper<MutableStyleProperties> create(const CSSProperty* properties, unsigned count);

    ~MutableStyleProperties();

    unsigned propertyCount() const { return m_propertyVector.size(); }
    bool isEmpty() const { return !propertyCount(); }
    PropertyReference propertyAt(unsigned index) const;

    PropertySetCSSStyleDeclaration* cssStyleDeclaration();

    bool addParsedProperties(const ParsedPropertyVector&);
    bool addParsedProperty(const CSSProperty&);

    // These expand shorthand properties into multiple properties.
    bool setProperty(CSSPropertyID, const std::string& value, bool important, CSSParserContext);
    bool setProperty(CSSPropertyID, const std::string& value, bool important = false);
    void setProperty(CSSPropertyID, std::shared_ptr<CSSValue>&&, bool important = false);

    // These do not. FIXME: This is too messy, we can do better.
    bool setProperty(CSSPropertyID, CSSValueID identifier, bool important = false);
    bool setProperty(CSSPropertyID, CSSPropertyID identifier, bool important = false);
    bool setProperty(const CSSProperty&, CSSProperty* slot = nullptr);

    bool removeProperty(CSSPropertyID, String* returnText = nullptr);
    void removeBlockProperties();
    bool removePropertiesInSet(const CSSPropertyID* set, unsigned length);

    void mergeAndOverrideOnConflict(const StyleProperties&);

    void clear();
    bool parseDeclaration(const std::string& styleDeclaration, CSSParserContext);

    CSSStyleDeclaration& ensureCSSStyleDeclaration();
    CSSStyleDeclaration& ensureInlineCSSStyleDeclaration(StyledElement& parentElement);

    int findPropertyIndex(CSSPropertyID) const;
    int findCustomPropertyIndex(const std::string& propertyName) const;
    
    std::vector<CSSProperty, 4> m_propertyVector;

    // Methods for querying and altering CSS custom properties.
    //bool setCustomProperty(const Document*, const std::string& propertyName, const std::string& value, bool important, CSSParserContext);
    bool removeCustomProperty(const std::string& propertyName, String* returnText = nullptr);

private:
    explicit MutableStyleProperties(CSSParserMode);
    explicit MutableStyleProperties(const StyleProperties&);
    MutableStyleProperties(const CSSProperty* properties, unsigned count);

    bool removeShorthandProperty(CSSPropertyID);
    CSSProperty* findCSSPropertyWithID(CSSPropertyID);
    CSSProperty* findCustomCSSPropertyWithName(const std::string&);
    std::unique_ptr<PropertySetCSSStyleDeclaration> m_cssomWrapper;

    friend class StyleProperties;
};

class DeferredStyleProperties final : public StylePropertiesBase {
public:
    ~DeferredStyleProperties();
    static std::reference_wrapper<DeferredStyleProperties> create(const CSSParserTokenRange&, CSSDeferredParser&);

    std::reference_wrapper<ImmutableStyleProperties> parseDeferredProperties();
    
private:
    DeferredStyleProperties(const CSSParserTokenRange&, CSSDeferredParser&);
    
    std::vector<CSSParserToken> m_tokens;
    std::reference_wrapper<CSSDeferredParser> m_parser;
};

inline ImmutableStyleProperties::PropertyReference ImmutableStyleProperties::propertyAt(unsigned index) const
{
    return PropertyReference(metadataArray()[index], valueArray()[index]);
}

inline MutableStyleProperties::PropertyReference MutableStyleProperties::propertyAt(unsigned index) const
{
    const CSSProperty& property = m_propertyVector[index];
    return PropertyReference(property.metadata(), property.value());
}

inline StyleProperties::PropertyReference StyleProperties::propertyAt(unsigned index) const
{
    if (is<MutableStyleProperties>(*this))
        return downcast<MutableStyleProperties>(*this).propertyAt(index);
    return downcast<ImmutableStyleProperties>(*this).propertyAt(index);
}

inline unsigned StyleProperties::propertyCount() const
{
    if (is<MutableStyleProperties>(*this))
        return downcast<MutableStyleProperties>(*this).propertyCount();
    return downcast<ImmutableStyleProperties>(*this).propertyCount();
}

inline void StylePropertiesBase::deref()
{
    if (!derefBase())
        return;

    if (is<MutableStyleProperties>(*this))
        delete downcast<MutableStyleProperties>(this);
    else if (is<ImmutableStyleProperties>(*this))
        delete downcast<ImmutableStyleProperties>(this);
    else
        delete downcast<DeferredStyleProperties>(this);
}

inline int StyleProperties::findPropertyIndex(CSSPropertyID propertyID) const
{
    if (is<MutableStyleProperties>(*this))
        return downcast<MutableStyleProperties>(*this).findPropertyIndex(propertyID);
    return downcast<ImmutableStyleProperties>(*this).findPropertyIndex(propertyID);
}

inline int StyleProperties::findCustomPropertyIndex(const std::string& propertyName) const
{
    if (is<MutableStyleProperties>(*this))
        return downcast<MutableStyleProperties>(*this).findCustomPropertyIndex(propertyName);
    return downcast<ImmutableStyleProperties>(*this).findCustomPropertyIndex(propertyName);
}

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::StyleProperties)
    static bool isType(const WebCore::StylePropertiesBase& set) { return set.type() != WebCore::DeferredPropertiesType; }
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::MutableStyleProperties)
    static bool isType(const WebCore::StylePropertiesBase& set) { return set.type() == WebCore::MutablePropertiesType; }
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::ImmutableStyleProperties)
    static bool isType(const WebCore::StylePropertiesBase& set) { return set.type() == WebCore::ImmutablePropertiesType; }
SPECIALIZE_TYPE_TRAITS_END()

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::DeferredStyleProperties)
    static bool isType(const WebCore::StylePropertiesBase& set) { return set.type() == WebCore::DeferredPropertiesType; }
SPECIALIZE_TYPE_TRAITS_END()
