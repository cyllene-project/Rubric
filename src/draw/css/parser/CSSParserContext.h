/*
 * Copyright (C) 2018 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "CSSParserMode.h"
#include <core/URL.h>
#include <string>

//#include "TextEncoding.h"
//#include <wtf/HashFunctions.h>
//#include <wtf/URLHash.h>
//#include <wtf/text/StringHash.h>

using namespace rubric;

namespace WebCore {

struct CSSParserContext {

public:
    CSSParserContext(const URL& baseURL = URL(), const std::string& charset = "");

    URL baseURL;
    std::string charset;
    bool textAutosizingEnabled { false };
    bool springTimingFunctionEnabled { false };
    bool constantPropertiesEnabled { false };
    bool colorFilterEnabled { false };
    bool deferredCSSParserEnabled { false };
    bool useSystemAppearance { false };

    URL completeURL(const std::string& url) const
    {
        if (url.empty())
            return URL();
        if (charset.empty())
            return URL(baseURL, url);
        //TextEncoding encoding(charset);
        //auto& encodingForURLParsing = encoding.encodingForFormSubmissionOrURLParsing();
        return URL(baseURL, url);
    }

    bool isContentOpaque { false };
};

bool operator==(const CSSParserContext&, const CSSParserContext&);
inline bool operator!=(const CSSParserContext& a, const CSSParserContext& b) { return !(a == b); }

const CSSParserContext& strictCSSParserContext();

struct CSSParserContextHash {
    static unsigned hash(const CSSParserContext& key)
    {
        unsigned hash = 0;
        if (!key.baseURL.empty())
            hash ^= WTF::URLHash::hash(key.baseURL);
        if (!key.charset.empty())
            hash ^= StringHash::hash(key.charset);
        unsigned bits = key.springTimingFunctionEnabled << 0
            & key.textAutosizingEnabled                     << 1
            & key.constantPropertiesEnabled                 << 2
            & key.colorFilterEnabled                        << 3
            & key.deferredCSSParserEnabled                  << 4
            & key.useSystemAppearance                       << 5;
        hash ^= WTF::intHash(bits);
        return hash;
    }
    static bool equal(const CSSParserContext& a, const CSSParserContext& b)
    {
        return a == b;
    }
    static const bool safeToCompareToEmptyOrDeleted = false;
};

} // namespace WebCore

namespace WTF {
template<> struct HashTraits<WebCore::CSSParserContext> : GenericHashTraits<WebCore::CSSParserContext> {
    static void constructDeletedValue(WebCore::CSSParserContext& slot) { new (NotNull, &slot.baseURL) URL(WTF::HashTableDeletedValue); }
    static bool isDeletedValue(const WebCore::CSSParserContext& value) { return value.baseURL.isHashTableDeletedValue(); }
    static WebCore::CSSParserContext emptyValue() { return WebCore::CSSParserContext(); }
};

template<> struct DefaultHash<WebCore::CSSParserContext> {
    typedef WebCore::CSSParserContextHash Hash;
};
} // namespace WTF
