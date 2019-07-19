/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2015 Apple Inc.
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



namespace WebCore {

class CSSPrimitiveValue;

// A primitive value representing a pair.  This is useful for properties like border-radius, background-size/position,
// and border-spacing (all of which are space-separated sets of two values).  At the moment we are only using it for
// border-radius and background-size, but (FIXME) border-spacing and background-position could be converted over to use
// it (eliminating some extra -webkit- internal properties).
class Pair {
public:
    enum class IdenticalValueEncoding : uint8_t {
        DoNotCoalesce,
        Coalesce
    };

    static ref_ptr<Pair> create(std::shared_ptr<CSSPrimitiveValue>&& first, std::shared_ptr<CSSPrimitiveValue>&& second)
    {
        return ref_ptr<Pair>(std::move(first), std::move(second));
    }
    static ref_ptr<Pair> create(std::shared_ptr<CSSPrimitiveValue>&& first, std::shared_ptr<CSSPrimitiveValue>&& second, IdenticalValueEncoding encoding)
    {
        return ref_ptr<Pair>(std::move(first), std::move(second), encoding);
    }
    CSSPrimitiveValue* first() const { return m_first.get(); }
    CSSPrimitiveValue* second() const { return m_second.get(); }

    std::string cssText() const
    {
        std::string first = this->first()->cssText();
        std::string second = this->second()->cssText();
        if (m_encoding == IdenticalValueEncoding::Coalesce && first == second)
            return first;
        return first + ' ' + second;
    }

    bool equals(const Pair& other) const { return compareCSSValuePtr(m_first, other.m_first) && compareCSSValuePtr(m_second, other.m_second); }

private:
    Pair(std::shared_ptr<CSSPrimitiveValue>&& first, std::shared_ptr<CSSPrimitiveValue>&& second)
        : m_first(std::move(first))
        , m_second(std::move(second))
    { }

    Pair(std::shared_ptr<CSSPrimitiveValue>&& first, std::shared_ptr<CSSPrimitiveValue>&& second, IdenticalValueEncoding encoding)
        : m_encoding(encoding)
        , m_first(std::move(first))
        , m_second(std::move(second))
    { }

    IdenticalValueEncoding m_encoding { IdenticalValueEncoding::Coalesce };
    std::shared_ptr<CSSPrimitiveValue> m_first;
    std::shared_ptr<CSSPrimitiveValue> m_second;
};

} // namespace
