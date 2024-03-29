/*
 * Copyright (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
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

class RectBase {
public:
    CSSPrimitiveValue* top() const { return m_top.get(); }
    CSSPrimitiveValue* right() const { return m_right.get(); }
    CSSPrimitiveValue* bottom() const { return m_bottom.get(); }
    CSSPrimitiveValue* left() const { return m_left.get(); }

    void setTop(std::shared_ptr<CSSPrimitiveValue>&& top) { m_top = std::move(top); }
    void setRight(std::shared_ptr<CSSPrimitiveValue>&& right) { m_right = std::move(right); }
    void setBottom(std::shared_ptr<CSSPrimitiveValue>&& bottom) { m_bottom = std::move(bottom); }
    void setLeft(std::shared_ptr<CSSPrimitiveValue>&& left) { m_left = std::move(left); }

    bool equals(const RectBase& other) const
    {
        return compareCSSValuePtr(m_top, other.m_top)
            && compareCSSValuePtr(m_right, other.m_right)
            && compareCSSValuePtr(m_left, other.m_left)
            && compareCSSValuePtr(m_bottom, other.m_bottom);
    }

protected:
    RectBase() = default;
    ~RectBase() = default;

private:
    std::shared_ptr<CSSPrimitiveValue> m_top;
    std::shared_ptr<CSSPrimitiveValue> m_right;
    std::shared_ptr<CSSPrimitiveValue> m_bottom;
    std::shared_ptr<CSSPrimitiveValue> m_left;
};

class Rect final : public RectBase, public RefCounted<Rect> {
public:
    static ref_ptr<Rect> create() { return adoptRef(*new Rect); }

    std::string cssText() const
    {
        return generateCSSString(top()->cssText(), right()->cssText(), bottom()->cssText(), left()->cssText());
    }

private:
    Rect() = default;
    static String generateCSSString(const std::string& top, const std::string& right, const std::string& bottom, const std::string& left)
    {
        return "rect(" + top + ", " + right + ", " + bottom + ", " + left + ')';
    }
};

class Quad final : public RectBase, public RefCounted<Quad> {
public:
    static ref_ptr<Quad> create() { return adoptRef(*new Quad); }

    std::string cssText() const
    {
        return generateCSSString(top()->cssText(), right()->cssText(), bottom()->cssText(), left()->cssText());
    }

private:
    Quad() = default;
    static String generateCSSString(const std::string& top, const std::string& right, const std::string& bottom, const std::string& left)
    {
        StringBuilder result;
        // reserve space for the four strings, plus three space separator characters.
        result.reserveCapacity(top.length() + right.length() + bottom.length() + left.length() + 3);
        result.append(top);
        if (right != top || bottom != top || left != top) {
            result.append(' ');
            result.append(right);
            if (bottom != top || right != left) {
                result.append(' ');
                result.append(bottom);
                if (left != right) {
                    result.append(' ');
                    result.append(left);
                }
            }
        }
        return result.toString();
    }
};

} // namespace WebCore
