/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2006, 2007 Apple Inc. All rights reserved.
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



#include <vector>#include <wtf/WeakPtr.h>

namespace WebCore {

class Document;
class HTMLStyleElement;
class Node;
class ShadowRoot;
class StyleSheet;
class CSSStyleSheet;

class StyleSheetList final : public RefCounted<StyleSheetList> {
public:
    static std::reference_wrapper<StyleSheetList> create(Document& document) { return adoptRef(*new StyleSheetList(document)); }
    static std::reference_wrapper<StyleSheetList> create(ShadowRoot& shadowRoot) { return adoptRef(*new StyleSheetList(shadowRoot)); }
    ~StyleSheetList();

    unsigned length() const;
    StyleSheet* item(unsigned index);

    CSSStyleSheet* namedItem(const std::atomic<std::string>&) const;
    std::vector<AtomString> supportedPropertyNames();

    Node* ownerNode() const;

    void detach();

private:
    StyleSheetList(Document&);
    StyleSheetList(ShadowRoot&);
    const std::vector<std::shared_ptr<StyleSheet>>& styleSheets() const;

    WeakPtr<Document> m_document;
    ShadowRoot* m_shadowRoot { nullptr };
    std::vector<std::shared_ptr<StyleSheet>> m_detachedStyleSheets;
};

} // namespace WebCore
