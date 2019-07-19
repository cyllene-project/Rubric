/*
 * Copyright (C) 2006 Rob Buis <buis@kde.org>
 * Copyright (C) 2008 Apple Inc. All right reserved.
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
//#include "IntPoint.h"
//#include "ResourceLoaderOptions.h"


namespace WebCore {

class CachedImage;
class CachedResourceLoader;
class Document;
class Element;
class SVGCursorElement;
class SVGElement;

class CSSCursorImageValue final : public CSSValue {
public:
    static std::reference_wrapper<CSSCursorImageValue> create(std::reference_wrapper<CSSValue>&& imageValue, bool hasHotSpot, const IntPoint& hotSpot, LoadedFromOpaqueSource loadedFromOpaqueSource)
    {
        return adoptRef(*new CSSCursorImageValue(std::move(imageValue), hasHotSpot, hotSpot, loadedFromOpaqueSource));
    }

    ~CSSCursorImageValue();

    bool hasHotSpot() const { return m_hasHotSpot; }

    IntPoint hotSpot() const
    {
        if (m_hasHotSpot)
            return m_hotSpot;
        return IntPoint(-1, -1);
    }

    const URL& imageURL() const { return m_originalURL; }

    std::string customCSSText() const;

    std::pair<CachedImage*, float> loadImage(CachedResourceLoader&, const ResourceLoaderOptions&);

    void removeReferencedElement(SVGElement*);

    bool equals(const CSSCursorImageValue&) const;

    void cursorElementRemoved(SVGCursorElement&);
    void cursorElementChanged(SVGCursorElement&);

private:
    CSSCursorImageValue(std::reference_wrapper<CSSValue>&& imageValue, bool hasHotSpot, const IntPoint& hotSpot, LoadedFromOpaqueSource);

    SVGCursorElement* updateCursorElement(const Document&);

    URL m_originalURL;
    std::reference_wrapper<CSSValue> m_imageValue;

    bool m_hasHotSpot;
    IntPoint m_hotSpot;
    std::unordered_set<SVGCursorElement*> m_cursorElements;
    LoadedFromOpaqueSource m_loadedFromOpaqueSource { LoadedFromOpaqueSource::No };
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSCursorImageValue, isCursorImageValue())
