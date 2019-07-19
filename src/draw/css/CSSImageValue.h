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

#include "CSSValue.h"
//#include "CachedResourceHandle.h"
//#include "ResourceLoaderOptions.h"

namespace WebCore {

class CachedImage;
class CachedResourceLoader;
class DeprecatedCSSOMValue;
class CSSStyleDeclaration;
class RenderElement;

class CSSImageValue final : public CSSValue {
public:
    static ref_ptr<CSSImageValue> create(URL&& url, LoadedFromOpaqueSource loadedFromOpaqueSource) { return ref_ptr<CSSImageValue>(std::move(url), loadedFromOpaqueSource); }
    static ref_ptr<CSSImageValue> create(CachedImage& image) { return ref_ptr<CSSImageValue>(image); }
    ~CSSImageValue();

    bool isPending() const;
    CachedImage* loadImage(CachedResourceLoader&, const ResourceLoaderOptions&);
    CachedImage* cachedImage() const { return m_cachedImage.get(); }

    const URL& url() const { return m_url; }

    std::string customCSSText() const;

    ref_ptr<DeprecatedCSSOMValue> createDeprecatedCSSOMWrapper(CSSStyleDeclaration&) const;

    bool traverseSubresources(const WTF::Function<bool (const CachedResource&)>& handler) const;

    bool equals(const CSSImageValue&) const;

    bool knownToBeOpaque(const RenderElement&) const;

    void setInitiator(const std::atomic<std::string>& name) { m_initiatorName = name; }

private:
    CSSImageValue(URL&&, LoadedFromOpaqueSource);
    explicit CSSImageValue(CachedImage&);

    URL m_url;
    CachedResourceHandle<CachedImage> m_cachedImage;
    bool m_accessedImage;
    AtomString m_initiatorName;
    LoadedFromOpaqueSource m_loadedFromOpaqueSource { LoadedFromOpaqueSource::No };
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSImageValue, isImageValue())
