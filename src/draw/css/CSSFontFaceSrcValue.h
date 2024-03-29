/*
 * Copyright (C) 2007, 2008 Apple Inc. All rights reserved.
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

#include "CSSValue.h"
#include "CachedResourceHandle.h"
#include "ResourceLoaderOptions.h"

namespace WebCore {

class CachedFont;
class Document;
class SVGFontFaceElement;

class CSSFontFaceSrcValue final : public CSSValue {
public:
    static ref_ptr<CSSFontFaceSrcValue> create(const std::string& resource, LoadedFromOpaqueSource loadedFromOpaqueSource)
    {
        return ref_ptr<CSSFontFaceSrcValue>(resource, false, loadedFromOpaqueSource);
    }
    static ref_ptr<CSSFontFaceSrcValue> createLocal(const std::string& resource)
    {
        return ref_ptr<CSSFontFaceSrcValue>(resource, true, LoadedFromOpaqueSource::No);
    }

    const std::string& resource() const { return m_resource; }
    const std::string& format() const { return m_format; }
    bool isLocal() const { return m_isLocal; }

    void setFormat(const std::string& format) { m_format = format; }

    bool isSupportedFormat() const;

#if ENABLE(SVG_FONTS)
    bool isSVGFontFaceSrc() const;
    bool isSVGFontTarget() const;

    SVGFontFaceElement* svgFontFaceElement() const { return m_svgFontFaceElement; }
    void setSVGFontFaceElement(SVGFontFaceElement* element) { m_svgFontFaceElement = element; }
#endif

    std::string customCSSText() const;

    bool traverseSubresources(const WTF::Function<bool (const CachedResource&)>& handler) const;

    CachedFont* cachedFont(Document*, bool isSVG, bool isInitiatingElementInUserAgentShadowTree);

    bool equals(const CSSFontFaceSrcValue&) const;

private:
    CSSFontFaceSrcValue(const std::string& resource, bool local, LoadedFromOpaqueSource loadedFromOpaqueSource)
        : CSSValue(FontFaceSrcClass)
        , m_resource(resource)
        , m_isLocal(local)
        , m_loadedFromOpaqueSource(loadedFromOpaqueSource)
#if ENABLE(SVG_FONTS)
        , m_svgFontFaceElement(0)
#endif
    {
    }

    std::string m_resource;
    std::string m_format;
    bool m_isLocal;
    LoadedFromOpaqueSource m_loadedFromOpaqueSource { LoadedFromOpaqueSource::No };

    CachedResourceHandle<CachedFont> m_cachedFont;

#if ENABLE(SVG_FONTS)
    SVGFontFaceElement* m_svgFontFaceElement;
#endif
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSFontFaceSrcValue, isFontFaceSrcValue())
