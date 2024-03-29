/*
 * Copyright (C) 2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008, 2014 Apple Inc. All rights reserved.
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

#include "CSSParserSelector.h"

#include "CSSSelector.h"
#include "CSSSelectorList.h"
#include "SelectorPseudoTypeMap.h"

#if COMPILER(MSVC)
// See https://msdn.microsoft.com/en-us/library/1wea5zwe.aspx
#pragma warning(disable: 4701)
#endif

namespace WebCore {

std::unique_ptr<CSSParserSelector> CSSParserSelector::parsePagePseudoSelector(StringView pseudoTypeString)
{
    CSSSelector::PagePseudoClassType pseudoType;
    if (equalLettersIgnoringASCIICase(pseudoTypeString, "first"))
        pseudoType = CSSSelector::PagePseudoClassFirst;
    else if (equalLettersIgnoringASCIICase(pseudoTypeString, "left"))
        pseudoType = CSSSelector::PagePseudoClassLeft;
    else if (equalLettersIgnoringASCIICase(pseudoTypeString, "right"))
        pseudoType = CSSSelector::PagePseudoClassRight;
    else
        return nullptr;

    auto selector = std::make_unique<CSSParserSelector>();
    selector->m_selector->setMatch(CSSSelector::PagePseudoClass);
    selector->m_selector->setPagePseudoType(pseudoType);
    return selector;
}

std::unique_ptr<CSSParserSelector> CSSParserSelector::parsePseudoElementSelector(StringView pseudoTypeString)
{
    auto pseudoType = CSSSelector::parsePseudoElementType(pseudoTypeString);
    if (pseudoType == CSSSelector::PseudoElementUnknown) {
        // FIXME-NEWPARSER: We can't add "slotted" to the map without breaking the old
        // parser, so this hack ensures the new parser still recognizes it. When the new
        // parser turns on, we can add "slotted" to the map and remove this code.
        if (pseudoTypeString.startsWithIgnoringASCIICase("slotted"))
            pseudoType = CSSSelector::PseudoElementSlotted;
        else
            return nullptr;
    }

    auto selector = std::make_unique<CSSParserSelector>();
    selector->m_selector->setMatch(CSSSelector::PseudoElement);
    selector->m_selector->setPseudoElementType(pseudoType);
    AtomString name;
    if (pseudoType != CSSSelector::PseudoElementWebKitCustomLegacyPrefixed)
        name = pseudoTypeString.convertToASCIILowercase();
    else {
        ASSERT_WITH_MESSAGE(equalLettersIgnoringASCIICase(pseudoTypeString, "-webkit-input-placeholder"), "-webkit-input-placeholder is the only LegacyPrefix pseudo type.");
        if (equalLettersIgnoringASCIICase(pseudoTypeString, "-webkit-input-placeholder"))
            name = AtomString("placeholder", AtomString::ConstructFromLiteral);
        else
            name = pseudoTypeString.convertToASCIILowercase();
    }
    selector->m_selector->setValue(name);
    return selector;
}

std::unique_ptr<CSSParserSelector> CSSParserSelector::parsePseudoClassSelector(StringView pseudoTypeString)
{
    auto pseudoType = parsePseudoClassAndCompatibilityElementString(pseudoTypeString);
    if (pseudoType.pseudoClass != CSSSelector::PseudoClassUnknown) {
        auto selector = std::make_unique<CSSParserSelector>();
        selector->m_selector->setMatch(CSSSelector::PseudoClass);
        selector->m_selector->setPseudoClassType(pseudoType.pseudoClass);
        return selector;
    }
    if (pseudoType.compatibilityPseudoElement != CSSSelector::PseudoElementUnknown) {
        auto selector = std::make_unique<CSSParserSelector>();
        selector->m_selector->setMatch(CSSSelector::PseudoElement);
        selector->m_selector->setPseudoElementType(pseudoType.compatibilityPseudoElement);
        selector->m_selector->setValue(pseudoTypeString.convertToASCIILowercase());
        return selector;
    }
    return nullptr;
}

CSSParserSelector::CSSParserSelector()
    : m_selector(std::make_unique<CSSSelector>())
{
}

CSSParserSelector::CSSParserSelector(const QualifiedName& tagQName)
    : m_selector(std::make_unique<CSSSelector>(tagQName))
{
}

CSSParserSelector::~CSSParserSelector()
{
    if (!m_tagHistory)
        return;
    std::vector<std::unique_ptr<CSSParserSelector>> toDelete;
    std::unique_ptr<CSSParserSelector> selector = std::move(m_tagHistory);
    while (true) {
        std::unique_ptr<CSSParserSelector> next = std::move(selector->m_tagHistory);
        toDelete.push_back(std::move(selector));
        if (!next)
            break;
        selector = std::move(next);
    }
}

void CSSParserSelector::adoptSelectorVector(std::vector<std::unique_ptr<CSSParserSelector>>&& selectorVector)
{
    m_selector->setSelectorList(std::make_unique<CSSSelectorList>(std::move(selectorVector)));
}

void CSSParserSelector::setLangArgumentList(std::unique_ptr<std::vector<AtomString>> argumentList)
{
    ASSERT_WITH_MESSAGE(!argumentList->isEmpty(), "No CSS Selector takes an empty argument list.");
    m_selector->setLangArgumentList(std::move(argumentList));
}

void CSSParserSelector::setSelectorList(std::unique_ptr<CSSSelectorList> selectorList)
{
    m_selector->setSelectorList(std::move(selectorList));
}

static bool selectorListMatchesPseudoElement(const CSSSelectorList* selectorList)
{
    if (!selectorList)
        return false;

    for (const CSSSelector* subSelector = selectorList->first(); subSelector; subSelector = CSSSelectorList::next(subSelector)) {
        for (const CSSSelector* selector = subSelector; selector; selector = selector->tagHistory()) {
            if (selector->matchesPseudoElement())
                return true;
            if (const CSSSelectorList* subselectorList = selector->selectorList()) {
                if (selectorListMatchesPseudoElement(subselectorList))
                    return true;
            }
        }
    }
    return false;
}

bool CSSParserSelector::matchesPseudoElement() const
{
    return m_selector->matchesPseudoElement() || selectorListMatchesPseudoElement(m_selector->selectorList());
}

void CSSParserSelector::insertTagHistory(CSSSelector::RelationType before, std::unique_ptr<CSSParserSelector> selector, CSSSelector::RelationType after)
{
    if (m_tagHistory)
        selector->setTagHistory(std::move(m_tagHistory));
    setRelation(before);
    selector->setRelation(after);
    m_tagHistory = std::move(selector);
}

void CSSParserSelector::appendTagHistory(CSSSelector::RelationType relation, std::unique_ptr<CSSParserSelector> selector)
{
    CSSParserSelector* end = this;
    while (end->tagHistory())
        end = end->tagHistory();

    end->setRelation(relation);
    end->setTagHistory(std::move(selector));
}

void CSSParserSelector::appendTagHistory(CSSParserSelectorCombinator relation, std::unique_ptr<CSSParserSelector> selector)
{
    CSSParserSelector* end = this;
    while (end->tagHistory())
        end = end->tagHistory();

    CSSSelector::RelationType selectorRelation;
    switch (relation) {
    case CSSParserSelectorCombinator::Child:
        selectorRelation = CSSSelector::Child;
        break;
    case CSSParserSelectorCombinator::DescendantSpace:
        selectorRelation = CSSSelector::DescendantSpace;
        break;
    case CSSParserSelectorCombinator::DirectAdjacent:
        selectorRelation = CSSSelector::DirectAdjacent;
        break;
    case CSSParserSelectorCombinator::IndirectAdjacent:
        selectorRelation = CSSSelector::IndirectAdjacent;
        break;
    }
    end->setRelation(selectorRelation);
    end->setTagHistory(std::move(selector));
}

void CSSParserSelector::prependTagSelector(const QualifiedName& tagQName, bool tagIsForNamespaceRule)
{
    auto second = std::make_unique<CSSParserSelector>();
    second->m_selector = std::move(m_selector);
    second->m_tagHistory = std::move(m_tagHistory);
    m_tagHistory = std::move(second);

    m_selector = std::make_unique<CSSSelector>(tagQName, tagIsForNamespaceRule);
    m_selector->setRelation(CSSSelector::Subselector);
}

std::unique_ptr<CSSParserSelector> CSSParserSelector::releaseTagHistory()
{
    setRelation(CSSSelector::Subselector);
    return std::move(m_tagHistory);
}

// FIXME-NEWPARSER: Add support for :host-context
bool CSSParserSelector::isHostPseudoSelector() const
{
    return match() == CSSSelector::PseudoClass && pseudoClassType() == CSSSelector::PseudoClassHost;
}

}

