/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
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

#include <vector>
#include <draw/css/ref_ptr.h>

using namespace rubric::draw::css;

namespace WebCore {

class CSSCustomPropertyValue;
struct CSSParserValue;
class CSSParserValueList;

class CSSValueList : public CSSValue {
public:
    typedef std::vector<ref_ptr<CSSValue>>::iterator iterator;
    typedef std::vector<ref_ptr<CSSValue>>::const_iterator const_iterator;

    static ref_ptr<CSSValueList> createCommaSeparated()
    {
        return ref_ptr<CSSValueList>(CommaSeparator);
    }
    static ref_ptr<CSSValueList> createSpaceSeparated()
    {
        return ref_ptr<CSSValueList>(SpaceSeparator);
    }
    static ref_ptr<CSSValueList> createSlashSeparated()
    {
        return ref_ptr<CSSValueList>(SlashSeparator);
    }

    size_t length() const { return m_values.size(); }
    CSSValue* item(size_t index) { return index < m_values.size() ? m_values[index].ptr() : nullptr; }
    const CSSValue* item(size_t index) const { return index < m_values.size() ? m_values[index].ptr() : nullptr; }
    CSSValue* itemWithoutBoundsCheck(size_t index) { return m_values[index].ptr(); }
    const CSSValue* itemWithoutBoundsCheck(size_t index) const { assert(index < m_values.size()); return m_values[index].ptr(); }

    const_iterator begin() const { return m_values.begin(); }
    const_iterator end() const { return m_values.end(); }
    iterator begin() { return m_values.begin(); }
    iterator end() { return m_values.end(); }

    void append(ref_ptr<CSSValue>&&);
    void prepend(ref_ptr<CSSValue>&&);
    bool removeAll(CSSValue*);
    bool hasValue(CSSValue*) const;
    ref_ptr<CSSValueList> copy();

    std::string customCSSText() const;
    bool equals(const CSSValueList&) const;
    bool equals(const CSSValue&) const;

    bool traverseSubresources(const std::function<bool (const CachedResource&)>& handler) const;

    unsigned separator() const { return m_valueListSeparator; }

protected:
    CSSValueList(ClassType, ValueListSeparator);

private:
    explicit CSSValueList(ValueListSeparator);

    std::vector<ref_ptr<CSSValue>> m_values;
};

inline void CSSValueList::append(ref_ptr<CSSValue>&& value)
{
    m_values.push_back(std::move(value));
}

inline void CSSValueList::prepend(ref_ptr<CSSValue>&& value)
{
    m_values.insert(m_values.begin(), std::move(value));
}

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_CSS_VALUE(CSSValueList, isValueList())
