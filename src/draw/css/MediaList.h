/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2006, 2008, 2009, 2010, 2012 Apple Inc. All rights reserved.
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

//#include "ExceptionOr.h"
#include "MediaQueryParserContext.h"
#include <memory>

#include <vector>

namespace WTF {
class TextStream;
}

namespace WebCore {

class CSSParser;
class CSSRule;
class CSSStyleSheet;
class Document;
class MediaQuery;

class MediaQuerySet final {
public:
    static ref_ptr<MediaQuerySet> create()
    {
        return adoptRef(*new MediaQuerySet);
    }
    static ref_ptr<MediaQuerySet> create(const std::string& mediaString, MediaQueryParserContext = MediaQueryParserContext());

    ~MediaQuerySet();

    bool set(const std::string&);
    bool add(const std::string&);
    bool remove(const std::string&);

    void addMediaQuery(MediaQuery&&);

    const std::vector<MediaQuery>& queryVector() const { return m_queries; }

    int lastLine() const { return m_lastLine; }
    void setLastLine(int lastLine) { m_lastLine = lastLine; }

    std::string mediaText() const;

    ref_ptr<MediaQuerySet> copy() const { return ref_ptr<MediaQuerySet>(*this); }

    void shrinkToFit();

private:
    MediaQuerySet();
    MediaQuerySet(const std::string& mediaQuery);
    MediaQuerySet(const MediaQuerySet&);

    int m_lastLine { 0 };
    std::vector<MediaQuery> m_queries;
};

class MediaList final {
public:
    static ref_ptr<MediaList> create(MediaQuerySet* mediaQueries, CSSStyleSheet* parentSheet)
    {
        return ref_ptr<MediaList>(mediaQueries, parentSheet);
    }
    static ref_ptr<MediaList> create(MediaQuerySet* mediaQueries, CSSRule* parentRule)
    {
        return ref_ptr<MediaList>(mediaQueries, parentRule);
    }

    ~MediaList();

    unsigned length() const { return m_mediaQueries->queryVector().size(); }
    std::string item(unsigned index) const;
    ExceptionOr<void> deleteMedium(const std::string& oldMedium);
    void appendMedium(const std::string& newMedium);

    std::string mediaText() const { return m_mediaQueries->mediaText(); }
    ExceptionOr<void> setMediaText(const std::string&);

    CSSRule* parentRule() const { return m_parentRule; }
    CSSStyleSheet* parentStyleSheet() const { return m_parentStyleSheet; }
    void clearParentStyleSheet() { assert(m_parentStyleSheet); m_parentStyleSheet = nullptr; }
    void clearParentRule() { assert(m_parentRule); m_parentRule = nullptr; }
    const MediaQuerySet* queries() const { return m_mediaQueries.get(); }

    void reattach(MediaQuerySet*);

private:
    MediaList();
    MediaList(MediaQuerySet*, CSSStyleSheet* parentSheet);
    MediaList(MediaQuerySet*, CSSRule* parentRule);

    std::shared_ptr<MediaQuerySet> m_mediaQueries;
    CSSStyleSheet* m_parentStyleSheet { nullptr };
    CSSRule* m_parentRule { nullptr };
};


WTF::TextStream& operator<<(WTF::TextStream&, const MediaQuerySet&);
WTF::TextStream& operator<<(WTF::TextStream&, const MediaList&);

} // namespace
