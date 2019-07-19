/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
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

#include "CSSPrimitiveValue.h"
namespace WebCore {

class Counter final {
public:
    static ref_ptr<Counter> create(ref_ptr<CSSPrimitiveValue>&& identifier, ref_ptr<CSSPrimitiveValue>&& listStyle, ref_ptr<CSSPrimitiveValue>&& separator)
    {
        return ref_ptr<Counter>(std::move(identifier), std::move(listStyle), std::move(separator));
    }

    std::string identifier() const { return m_identifier->stringValue(); }
    std::string listStyle() const { return m_listStyle->stringValue(); }
    std::string separator() const { return m_separator->stringValue(); }

    const CSSPrimitiveValue& identifierValue() const { return m_identifier; }
    const CSSPrimitiveValue& listStyleValue() const { return m_listStyle; }
    const CSSPrimitiveValue& separatorValue() const { return m_separator; }
    
    CSSValueID listStyleIdent() const { return m_listStyle->valueID(); }

    void setIdentifier(ref_ptr<CSSPrimitiveValue>&& identifier) { m_identifier = std::move(identifier); }
    void setListStyle(ref_ptr<CSSPrimitiveValue>&& listStyle) { m_listStyle = std::move(listStyle); }
    void setSeparator(ref_ptr<CSSPrimitiveValue>&& separator) { m_separator = std::move(separator); }

    bool equals(const Counter& other) const
    {
        return identifier() == other.identifier()
            && listStyle() == other.listStyle()
            && separator() == other.separator();
    }

private:
    Counter(ref_ptr<CSSPrimitiveValue>&& identifier, ref_ptr<CSSPrimitiveValue>&& listStyle, ref_ptr<CSSPrimitiveValue>&& separator)
        : m_identifier(std::move(identifier))
        , m_listStyle(std::move(listStyle))
        , m_separator(std::move(separator))
    {
    }

    ref_ptr<CSSPrimitiveValue> m_identifier; // string
    ref_ptr<CSSPrimitiveValue> m_listStyle; // ident
    ref_ptr<CSSPrimitiveValue> m_separator; // string
};

} // namespace WebCore
