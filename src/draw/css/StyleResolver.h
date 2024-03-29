/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013 Apple Inc. All rights reserved.
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
 *
 */

#pragma once

#include "CSSSelector.h"
#include "CSSToLengthConversionData.h"
#include "CSSToStyleMap.h"
#include "DocumentRuleSets.h"
#include "InspectorCSSOMWrappers.h"
#include "MediaQueryEvaluator.h"
//#include "RenderStyle.h"
#include "RuleSet.h"
#include "SelectorChecker.h"
#include <bitset>
#include <memory>
//#include <wtf/Bitmap.h>
#include <vector>

namespace WebCore {

class CSSCursorImageValue;
class CSSFontFace;
class CSSFontFaceRule;
class CSSFontValue;
class CSSImageGeneratorValue;
class CSSImageSetValue;
class CSSImageValue;
class CSSPageRule;
class CSSPrimitiveValue;
class CSSProperty;
class CSSStyleSheet;
class CSSValue;
class ContainerNode;
class Document;
class Element;
class Frame;
class FrameView;
class KeyframeList;
class KeyframeValue;
class MediaQueryEvaluator;
class Node;
class RenderScrollbar;
class RuleData;
class RuleSet;
class SelectorFilter;
class Settings;
class StyleCachedImage;
class StyleGeneratedImage;
class StyleImage;
class StyleRuleKeyframe;
class StyleProperties;
class StyleRule;
class StyleRuleKeyframes;
class StyleRulePage;
class StyleSheet;
class StyleSheetList;
class StyledElement;
class SVGSVGElement;
class ViewportStyleResolver;
struct ResourceLoaderOptions;

// MatchOnlyUserAgentRules is used in media queries, where relative units
// are interpreted according to the document root element style, and styled only
// from the User Agent Stylesheet rules.

enum class RuleMatchingBehavior: uint8_t {
    MatchAllRules,
    MatchAllRulesExcludingSMIL,
    MatchOnlyUserAgentRules,
};

enum class CascadeLevel: uint8_t {
    UserAgentLevel,
    AuthorLevel,
    UserLevel
};

class PseudoStyleRequest {
public:
    PseudoStyleRequest(PseudoId pseudoId, RenderScrollbar* scrollbar = nullptr, ScrollbarPart scrollbarPart = NoPart)
        : pseudoId(pseudoId)
        , scrollbarPart(scrollbarPart)
        , scrollbar(scrollbar)
    {
    }

    PseudoId pseudoId;
    ScrollbarPart scrollbarPart;
    RenderScrollbar* scrollbar;
};

struct ElementStyle {
    ElementStyle(std::unique_ptr<RenderStyle> renderStyle, std::unique_ptr<Style::Relations> relations = { })
        : renderStyle(std::move(renderStyle))
        , relations(std::move(relations))
    { }

    std::unique_ptr<RenderStyle> renderStyle;
    std::unique_ptr<Style::Relations> relations;
};

// This class selects a RenderStyle for a given element based on a collection of stylesheets.
class StyleResolver {
    StyleResolver(const StyleResolver&) = delete;
        StyleResolver& operator=(const StyleResolver&) = delete;
public:
    StyleResolver(Document&);
    ~StyleResolver();

    ElementStyle styleForElement(const Element&, const RenderStyle* parentStyle, const RenderStyle* parentBoxStyle = nullptr, RuleMatchingBehavior = RuleMatchingBehavior::MatchAllRules, const SelectorFilter* = nullptr);

    void keyframeStylesForAnimation(const Element&, const RenderStyle*, KeyframeList&);

    std::unique_ptr<RenderStyle> pseudoStyleForElement(const Element&, const PseudoStyleRequest&, const RenderStyle& parentStyle, const SelectorFilter* = nullptr);

    std::unique_ptr<RenderStyle> styleForPage(int pageIndex);
    std::unique_ptr<RenderStyle> defaultStyleForElement();

    RenderStyle* style() const { return m_state.style(); }
    const RenderStyle* parentStyle() const { return m_state.parentStyle(); }
    const RenderStyle* rootElementStyle() const { return m_state.rootElementStyle(); }
    const Element* element() { return m_state.element(); }
    Document& document() { return m_document; }
    const Document& document() const { return m_document; }
    const Settings& settings() const { return m_document.settings(); }

    void appendAuthorStyleSheets(const std::vector<std::shared_ptr<CSSStyleSheet>>&);

    DocumentRuleSets& ruleSets() { return m_ruleSets; }
    const DocumentRuleSets& ruleSets() const { return m_ruleSets; }

    const MediaQueryEvaluator& mediaQueryEvaluator() const { return m_mediaQueryEvaluator; }

    RenderStyle* overrideDocumentElementStyle() const { return m_overrideDocumentElementStyle; }
    void setOverrideDocumentElementStyle(RenderStyle* style) { m_overrideDocumentElementStyle = style; }

    void addCurrentSVGFontFaceRules();
    static void adjustSVGElementStyle(const SVGElement&, RenderStyle&);

    void setNewStateWithElement(const Element&);
    std::unique_ptr<RenderStyle> styleForKeyframe(const RenderStyle*, const StyleRuleKeyframe*, KeyframeValue&);
    bool isAnimationNameValid(const std::string&);

public:
    // These methods will give back the set of rules that matched for a given element (or a pseudo-element).
    enum CSSRuleFilter {
        UAAndUserCSSRules   = 1 << 1,
        AuthorCSSRules      = 1 << 2,
        EmptyCSSRules       = 1 << 3,
        AllButEmptyCSSRules = UAAndUserCSSRules | AuthorCSSRules,
        AllCSSRules         = AllButEmptyCSSRules | EmptyCSSRules,
    };
    std::vector<std::shared_ptr<StyleRule>> styleRulesForElement(const Element*, unsigned rulesToInclude = AllButEmptyCSSRules);
    std::vector<std::shared_ptr<StyleRule>> pseudoStyleRulesForElement(const Element*, PseudoId, unsigned rulesToInclude = AllButEmptyCSSRules);

public:
    struct MatchResult;

    void applyPropertyToStyle(CSSPropertyID, CSSValue*, std::unique_ptr<RenderStyle>);
    void applyPropertyToCurrentStyle(CSSPropertyID, CSSValue*);

    void updateFont();
    void initializeFontStyle();

    void setFontSize(FontCascadeDescription&, float size);

public:
    bool useSVGZoomRules() const;
    bool useSVGZoomRulesForLength() const;

    static bool colorFromPrimitiveValueIsDerivedFromElement(const CSSPrimitiveValue&);
    Color colorFromPrimitiveValue(const CSSPrimitiveValue&, bool forVisitedLink = false) const;

    bool hasSelectorForId(const std::atomic<std::string>&) const;
    bool hasSelectorForAttribute(const Element&, const std::atomic<std::string>&) const;

    void addViewportDependentMediaQueryResult(const MediaQueryExpression&, bool result);
    bool hasViewportDependentMediaQueries() const { return !m_viewportDependentMediaQueryResults.isEmpty(); }
    bool hasMediaQueriesAffectedByViewportChange() const;

    void addAccessibilitySettingsDependentMediaQueryResult(const MediaQueryExpression&, bool result);
    bool hasAccessibilitySettingsDependentMediaQueries() const { return !m_accessibilitySettingsDependentMediaQueryResults.isEmpty(); }
    bool hasMediaQueriesAffectedByAccessibilitySettingsChange() const;

    void addAppearanceDependentMediaQueryResult(const MediaQueryExpression&, bool result);
    bool hasAppearanceDependentMediaQueries() const { return !m_appearanceDependentMediaQueryResults.isEmpty(); }
    bool hasMediaQueriesAffectedByAppearanceChange() const;

    void addKeyframeStyle(ref_ptr<StyleRuleKeyframes>&&);

    bool usesFirstLineRules() const { return m_ruleSets.features().usesFirstLineRules; }
    bool usesFirstLetterRules() const { return m_ruleSets.features().usesFirstLetterRules; }
    
    void invalidateMatchedPropertiesCache();

    void clearCachedPropertiesAffectedByViewportUnits();

    bool createFilterOperations(const CSSValue& inValue, FilterOperations& outOperations);

    struct RuleRange {
        RuleRange(int& firstRuleIndex, int& lastRuleIndex): firstRuleIndex(firstRuleIndex), lastRuleIndex(lastRuleIndex) { }
        int& firstRuleIndex;
        int& lastRuleIndex;
    };

    struct MatchRanges {
        MatchRanges() : firstUARule(-1), lastUARule(-1), firstAuthorRule(-1), lastAuthorRule(-1), firstUserRule(-1), lastUserRule(-1) { }
        int firstUARule;
        int lastUARule;
        int firstAuthorRule;
        int lastAuthorRule;
        int firstUserRule;
        int lastUserRule;
        RuleRange UARuleRange() { return RuleRange(firstUARule, lastUARule); }
        RuleRange authorRuleRange() { return RuleRange(firstAuthorRule, lastAuthorRule); }
        RuleRange userRuleRange() { return RuleRange(firstUserRule, lastUserRule); }
    };

    struct MatchedProperties {
        MatchedProperties();
        ~MatchedProperties();
        
        std::shared_ptr<StyleProperties> properties;
        uint16_t linkMatchType;
        uint16_t whitelistType;
        Style::ScopeOrdinal styleScopeOrdinal;
    };

    struct MatchResult {
        MatchResult() : isCacheable(true) { }
        std::vector<StyleRule*, 64> matchedRules;
        MatchRanges ranges;
        bool isCacheable;

        const std::vector<MatchedProperties, 64>& matchedProperties() const { return m_matchedProperties; }

        void addMatchedProperties(const StyleProperties&, StyleRule* = nullptr, unsigned linkMatchType = SelectorChecker::MatchAll, PropertyWhitelistType = PropertyWhitelistNone, Style::ScopeOrdinal = Style::ScopeOrdinal::Element);
    private:
        std::vector<MatchedProperties, 64> m_matchedProperties;
    };
    
    class CascadedProperties {

    public:
        CascadedProperties(TextDirection, WritingMode);

        struct Property {
            void apply(StyleResolver&, ApplyCascadedPropertyState&);

            CSSPropertyID id;
            CascadeLevel level;
            Style::ScopeOrdinal styleScopeOrdinal;
            CSSValue* cssValue[3];
        };

        bool hasProperty(CSSPropertyID) const;
        Property& property(CSSPropertyID);

        void addNormalMatches(const MatchResult&, int startIndex, int endIndex, bool inheritedOnly = false);
        void addImportantMatches(const MatchResult&, int startIndex, int endIndex, bool inheritedOnly = false);

        void applyDeferredProperties(StyleResolver&, ApplyCascadedPropertyState&);

        std::unordered_map<AtomString, Property>& customProperties() { return m_customProperties; }
        bool hasCustomProperty(const std::string&) const;
        Property customProperty(const std::string&) const;
        
    private:
        void addMatch(const MatchResult&, unsigned index, bool isImportant, bool inheritedOnly);
        void set(CSSPropertyID, CSSValue&, unsigned linkMatchType, CascadeLevel, Style::ScopeOrdinal);
        void setDeferred(CSSPropertyID, CSSValue&, unsigned linkMatchType, CascadeLevel, Style::ScopeOrdinal);
        static void setPropertyInternal(Property&, CSSPropertyID, CSSValue&, unsigned linkMatchType, CascadeLevel, Style::ScopeOrdinal);

        Property m_properties[numCSSProperties + 2];
        std::bitset<numCSSProperties + 2> m_propertyIsPresent;

        std::vector<Property, 8> m_deferredProperties;
        std::unordered_map<AtomString, Property> m_customProperties;

        TextDirection m_direction;
        WritingMode m_writingMode;
    };

    void applyCascadedProperties(int firstProperty, int lastProperty, ApplyCascadedPropertyState&);
    void applyCascadedCustomProperty(const std::string& name, ApplyCascadedPropertyState&);

private:
    // This function fixes up the default font size if it detects that the current generic font family has changed. -dwh
    void checkForGenericFamilyChange(RenderStyle&, const RenderStyle* parentStyle);
    void checkForZoomChange(RenderStyle&, const RenderStyle* parentStyle);
    void checkForTextSizeAdjust(RenderStyle&);

    void adjustRenderStyle(RenderStyle&, const RenderStyle& parentStyle, const RenderStyle* parentBoxStyle, const Element*);
    void adjustRenderStyleForSiteSpecificQuirks(RenderStyle&, const Element&);

    std::unique_ptr<GridPosition> adjustNamedGridItemPosition(const NamedGridAreaMap&, const NamedGridLinesMap&, const GridPosition&, GridPositionSide) const;
    
    void adjustStyleForInterCharacterRuby();
    
    bool fastRejectSelector(const RuleData&) const;

    enum ShouldUseMatchedPropertiesCache { DoNotUseMatchedPropertiesCache = 0, UseMatchedPropertiesCache };
    void applyMatchedProperties(const MatchResult&, const Element&, ShouldUseMatchedPropertiesCache = UseMatchedPropertiesCache);

    enum CustomPropertyCycleTracking { Enabled = 0, Disabled };
    template<CustomPropertyCycleTracking>
    inline void applyCascadedPropertiesImpl(int firstProperty, int lastProperty, ApplyCascadedPropertyState&);

    void cascadeMatches(CascadedProperties&, const MatchResult&, bool important, int startIndex, int endIndex, bool inheritedOnly);

    void matchPageRules(MatchResult&, RuleSet*, bool isLeftPage, bool isFirstPage, const std::string& pageName);
    void matchPageRulesForList(std::vector<StyleRulePage*>& matchedRules, const std::vector<StyleRulePage*>&, bool isLeftPage, bool isFirstPage, const std::string& pageName);

    bool isLeftPage(int pageIndex) const;
    bool isRightPage(int pageIndex) const { return !isLeftPage(pageIndex); }
    bool isFirstPage(int pageIndex) const;
    std::string pageName(int pageIndex) const;

    DocumentRuleSets m_ruleSets;

    typedef std::unordered_map<AtomStringImpl*, std::shared_ptr<StyleRuleKeyframes>> KeyframesRuleMap;
    KeyframesRuleMap m_keyframesRuleMap;

public:
    typedef std::unordered_map<CSSPropertyID, std::shared_ptr<CSSValue>> PendingImagePropertyMap;

    class State {
    public:
        State() { }
        State(const Element&, const RenderStyle* parentStyle, const RenderStyle* documentElementStyle = nullptr, const SelectorFilter* = nullptr);

    public:
        void clear();

        const Element* element() const { return m_element; }

        void setStyle(std::unique_ptr<RenderStyle>);
        RenderStyle* style() const { return m_style.get(); }
        std::unique_ptr<RenderStyle> takeStyle() { return std::move(m_style); }

        void setParentStyle(std::unique_ptr<RenderStyle>);
        const RenderStyle* parentStyle() const { return m_parentStyle; }
        const RenderStyle* rootElementStyle() const { return m_rootElementStyle; }

        InsideLink elementLinkState() const { return m_elementLinkState; }

        void setApplyPropertyToRegularStyle(bool isApply) { m_applyPropertyToRegularStyle = isApply; }
        void setApplyPropertyToVisitedLinkStyle(bool isApply) { m_applyPropertyToVisitedLinkStyle = isApply; }
        bool applyPropertyToRegularStyle() const { return m_applyPropertyToRegularStyle; }
        bool applyPropertyToVisitedLinkStyle() const { return m_applyPropertyToVisitedLinkStyle; }

        void setFontDirty(bool isDirty) { m_fontDirty = isDirty; }
        bool fontDirty() const { return m_fontDirty; }
        void setFontSizeHasViewportUnits(bool hasViewportUnits) { m_fontSizeHasViewportUnits = hasViewportUnits; }
        bool fontSizeHasViewportUnits() const { return m_fontSizeHasViewportUnits; }

        void cacheBorderAndBackground();
        bool hasUAAppearance() const { return m_hasUAAppearance; }
        BorderData borderData() const { return m_borderData; }
        FillLayer backgroundData() const { return m_backgroundData; }
        const Color& backgroundColor() const { return m_backgroundColor; }

        const FontCascadeDescription& fontDescription() { return m_style->fontDescription(); }
        const FontCascadeDescription& parentFontDescription() { return m_parentStyle->fontDescription(); }
        void setFontDescription(FontCascadeDescription&& fontDescription) { m_fontDirty |= m_style->setFontDescription(std::move(fontDescription)); }
        void setZoom(float f) { m_fontDirty |= m_style->setZoom(f); }
        void setEffectiveZoom(float f) { m_fontDirty |= m_style->setEffectiveZoom(f); }
        void setWritingMode(WritingMode writingMode) { m_fontDirty |= m_style->setWritingMode(writingMode); }
        void setTextOrientation(TextOrientation textOrientation) { m_fontDirty |= m_style->setTextOrientation(textOrientation); }

        bool useSVGZoomRules() const { return m_element && m_element->isSVGElement(); }

        const CSSToLengthConversionData& cssToLengthConversionData() const { return m_cssToLengthConversionData; }

        CascadeLevel cascadeLevel() const { return m_cascadeLevel; }
        void setCascadeLevel(CascadeLevel level) { m_cascadeLevel = level; }
        Style::ScopeOrdinal styleScopeOrdinal() const { return m_styleScopeOrdinal; }
        void setStyleScopeOrdinal(Style::ScopeOrdinal styleScopeOrdinal) { m_styleScopeOrdinal = styleScopeOrdinal; }

        CascadedProperties* authorRollback() const { return m_authorRollback.get(); }
        CascadedProperties* userRollback() const { return m_userRollback.get(); }
        
        void setAuthorRollback(std::unique_ptr<CascadedProperties>& rollback) { m_authorRollback = std::move(rollback); }
        void setUserRollback(std::unique_ptr<CascadedProperties>& rollback) { m_userRollback = std::move(rollback); }

        const SelectorFilter* selectorFilter() const { return m_selectorFilter; }
        
    private:
        void updateConversionData();

        const Element* m_element { nullptr };
        std::unique_ptr<RenderStyle> m_style;
        const RenderStyle* m_parentStyle { nullptr };
        std::unique_ptr<RenderStyle> m_ownedParentStyle;
        const RenderStyle* m_rootElementStyle { nullptr };
        std::unique_ptr<CascadedProperties> m_authorRollback;
        std::unique_ptr<CascadedProperties> m_userRollback;

        const SelectorFilter* m_selectorFilter { nullptr };

        BorderData m_borderData;
        FillLayer m_backgroundData { FillLayerType::Background };
        Color m_backgroundColor;

        CSSToLengthConversionData m_cssToLengthConversionData;

        Style::ScopeOrdinal m_styleScopeOrdinal { Style::ScopeOrdinal::Element };

        InsideLink m_elementLinkState { InsideLink::NotInside };
        CascadeLevel m_cascadeLevel { CascadeLevel::UserAgentLevel };

        bool m_applyPropertyToRegularStyle { true };
        bool m_applyPropertyToVisitedLinkStyle { false };
        bool m_fontDirty { false };
        bool m_fontSizeHasViewportUnits { false };
        bool m_hasUAAppearance { false };
    };

    State& state() { return m_state; }
    const State& state() const { return m_state; }

    std::shared_ptr<StyleImage> styleImage(CSSValue&);

    bool applyPropertyToRegularStyle() const { return m_state.applyPropertyToRegularStyle(); }
    bool applyPropertyToVisitedLinkStyle() const { return m_state.applyPropertyToVisitedLinkStyle(); }

    CascadeLevel cascadeLevel() const { return m_state.cascadeLevel(); }
    void setCascadeLevel(CascadeLevel level) { m_state.setCascadeLevel(level); }
    
    CascadedProperties* cascadedPropertiesForRollback(const MatchResult&);

    CSSToStyleMap* styleMap() { return &m_styleMap; }
    InspectorCSSOMWrappers& inspectorCSSOMWrappers() { return m_inspectorCSSOMWrappers; }
    const FontCascadeDescription& fontDescription() { return m_state.fontDescription(); }
    const FontCascadeDescription& parentFontDescription() { return m_state.parentFontDescription(); }
    void setFontDescription(FontCascadeDescription&& fontDescription) { m_state.setFontDescription(std::move(fontDescription)); }
    void setZoom(float f) { m_state.setZoom(f); }
    void setEffectiveZoom(float f) { m_state.setEffectiveZoom(f); }
    void setWritingMode(WritingMode writingMode) { m_state.setWritingMode(writingMode); }
    void setTextOrientation(TextOrientation textOrientation) { m_state.setTextOrientation(textOrientation); }

    std::shared_ptr<CSSValue> resolvedVariableValue(CSSPropertyID, const CSSValue&, ApplyCascadedPropertyState&) const;

private:
    void cacheBorderAndBackground();

    void applyProperty(CSSPropertyID, CSSValue*, ApplyCascadedPropertyState&, SelectorChecker::LinkMatchMask = SelectorChecker::MatchDefault);

    void applySVGProperty(CSSPropertyID, CSSValue*);

    static unsigned computeMatchedPropertiesHash(const MatchedProperties*, unsigned size);
    struct MatchedPropertiesCacheItem {
        std::vector<MatchedProperties> matchedProperties;
        MatchRanges ranges;
        std::unique_ptr<RenderStyle> renderStyle;
        std::unique_ptr<RenderStyle> parentRenderStyle;
        
        MatchedPropertiesCacheItem(const MatchResult& matchResult, const RenderStyle* style, const RenderStyle* parentStyle)
            : ranges(matchResult.ranges)
            , renderStyle(RenderStyle::clonePtr(*style))
            , parentRenderStyle(RenderStyle::clonePtr(*parentStyle))
        {
            // Assign rather than copy-construct so we only allocate as much vector capacity as needed.
            matchedProperties = matchResult.matchedProperties();
        }
        MatchedPropertiesCacheItem() = default;
    };
    const MatchedPropertiesCacheItem* findFromMatchedPropertiesCache(unsigned hash, const MatchResult&);
    void addToMatchedPropertiesCache(const RenderStyle*, const RenderStyle* parentStyle, unsigned hash, const MatchResult&);

    // Every N additions to the matched declaration cache trigger a sweep where entries holding
    // the last reference to a style declaration are garbage collected.
    void sweepMatchedPropertiesCache();

    void adjustRenderStyleForTextAutosizing(RenderStyle&, const Element&);

    typedef std::unordered_map<unsigned, MatchedPropertiesCacheItem> MatchedPropertiesCache;
    MatchedPropertiesCache m_matchedPropertiesCache;

    Timer m_matchedPropertiesCacheSweepTimer;

    MediaQueryEvaluator m_mediaQueryEvaluator;
    std::unique_ptr<RenderStyle> m_rootDefaultStyle;

    Document& m_document;

    RenderStyle* m_overrideDocumentElementStyle { nullptr };

    std::vector<MediaQueryResult> m_viewportDependentMediaQueryResults;
    std::vector<MediaQueryResult> m_accessibilitySettingsDependentMediaQueryResults;
    std::vector<MediaQueryResult> m_appearanceDependentMediaQueryResults;

    CSSToStyleMap m_styleMap;
    InspectorCSSOMWrappers m_inspectorCSSOMWrappers;

    State m_state;

    unsigned m_matchedPropertiesCacheAdditionsSinceLastSweep { 0 };

    bool m_matchAuthorAndUserStyles { true };
    // See if we still have crashes where StyleResolver gets deleted early.
    bool m_isDeleted { false };

    friend bool operator==(const MatchedProperties&, const MatchedProperties&);
    friend bool operator!=(const MatchedProperties&, const MatchedProperties&);
    friend bool operator==(const MatchRanges&, const MatchRanges&);
    friend bool operator!=(const MatchRanges&, const MatchRanges&);
};

// State to use when applying properties, to keep track of which custom and high-priority
// properties have been resolved.
struct ApplyCascadedPropertyState {
    StyleResolver* styleResolver;
    StyleResolver::CascadedProperties* cascade;
    const StyleResolver::MatchResult* matchResult;
    Bitmap<numCSSProperties> appliedProperties = { };
    std::unordered_set<std::string> appliedCustomProperties = { };
    Bitmap<numCSSProperties> inProgressProperties = { };
    std::unordered_set<std::string> inProgressPropertiesCustom = { };
};


inline bool StyleResolver::hasSelectorForAttribute(const Element& element, const AtomString &attributeName) const
{
    assert(!attributeName.isEmpty());
    if (element.isHTMLElement())
        return m_ruleSets.features().attributeCanonicalLocalNamesInRules.contains(attributeName);
    return m_ruleSets.features().attributeLocalNamesInRules.contains(attributeName);
}

inline bool StyleResolver::hasSelectorForId(const std::atomic<std::string>& idValue) const
{
    assert(!idValue.isEmpty());
    return m_ruleSets.features().idsInRules.contains(idValue);
}

} // namespace WebCore
