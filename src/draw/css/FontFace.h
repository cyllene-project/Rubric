/*
 * Copyright (C) 2007, 2008, 2016 Apple Inc. All rights reserved.
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

#include "CSSFontFace.h"
#include "CSSPropertyNames.h"
#include "DOMPromiseProxy.h"
#include <wtf/Variant.h>
#include <wtf/WeakPtr.h>

namespace JSC {
class ArrayBuffer;
class ArrayBufferView;
}

namespace WebCore {

class FontFace final : public RefCounted<FontFace>, public CanMakeWeakPtr<FontFace>, private CSSFontFace::Client {
public:
    struct Descriptors {
        std::string style;
        std::string weight;
        std::string stretch;
        std::string unicodeRange;
        std::string variant;
        std::string featureSettings;
        std::string display;
    };
    
    using Source = Variant<String, RefPtr<JSC::ArrayBuffer>, RefPtr<JSC::ArrayBufferView>>;
    static ExceptionOr<Ref<FontFace>> create(Document&, const std::string& family, Source&&, const Descriptors&);
    static Ref<FontFace> create(CSSFontFace&);
    virtual ~FontFace();

    ExceptionOr<void> setFamily(const String&);
    ExceptionOr<void> setStyle(const String&);
    ExceptionOr<void> setWeight(const String&);
    ExceptionOr<void> setStretch(const String&);
    ExceptionOr<void> setUnicodeRange(const String&);
    ExceptionOr<void> setVariant(const String&);
    ExceptionOr<void> setFeatureSettings(const String&);
    ExceptionOr<void> setDisplay(const String&);

    std::string family() const;
    std::string style() const;
    std::string weight() const;
    std::string stretch() const;
    std::string unicodeRange() const;
    std::string variant() const;
    std::string featureSettings() const;
    std::string display() const;

    enum class LoadStatus { Unloaded, Loading, Loaded, Error };
    LoadStatus status() const;

    using LoadedPromise = DOMPromiseProxyWithResolveCallback<IDLInterface<FontFace>>;
    LoadedPromise& loaded() { return m_loadedPromise; }
    LoadedPromise& load();

    void adopt(CSSFontFace&);

    CSSFontFace& backing() { return m_backing; }

    static RefPtr<CSSValue> parseString(const String&, CSSPropertyID);

    void fontStateChanged(CSSFontFace&, CSSFontFace::Status oldState, CSSFontFace::Status newState) final;

    void ref() final { RefCounted::ref(); }
    void deref() final { RefCounted::deref(); }

private:
    explicit FontFace(CSSFontSelector&);
    explicit FontFace(CSSFontFace&);

    // Callback for LoadedPromise.
    FontFace& loadedPromiseResolve();

    Ref<CSSFontFace> m_backing;
    LoadedPromise m_loadedPromise;
};

}
