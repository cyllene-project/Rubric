// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_CSSPARSERIMPL_H
#define RUBRIC_CSSPARSERIMPL_H

#include "CSSParserContext.h"
#include "CSSParser.h"
#include <draw/css/StyleSheetContents.h>

namespace rubric::draw::css {

    class CSSParserImpl {
        CSSParserImpl(const CSSParserContext&, const String&, StyleSheetContents* = nullptr, CSSParserObserverWrapper* = nullptr, CSSParser::RuleParsing = CSSParser::RuleParsing::Normal);

        enum AllowedRulesType {
            // As per css-syntax, css-cascade and css-namespaces, @charset rules
            // must come first, followed by @import then @namespace.
            // AllowImportRules actually means we allow @import and any rules thay
            // may follow it, i.e. @namespace rules and regular rules.
            // AllowCharsetRules and AllowNamespaceRules behave similarly.
            AllowCharsetRules,
            AllowImportRules,
            AllowNamespaceRules,
            RegularRules,
            KeyframeRules,
            ApplyRules, // For @apply inside style rules
            NoRules, // For parsing at-rules inside declaration lists
        };

        static CSSParser::ParseResult parseValue(MutableStyleProperties*, CSSPropertyID, const String&, bool important, const CSSParserContext&);
        static CSSParser::ParseResult parseCustomPropertyValue(MutableStyleProperties*, const AtomString& propertyName, const String&, bool important, const CSSParserContext&);
        static Ref<ImmutableStyleProperties> parseInlineStyleDeclaration(const String&, const Element*);
        static bool parseDeclarationList(MutableStyleProperties*, const String&, const CSSParserContext&);
        static RefPtr<StyleRuleBase> parseRule(const String&, const CSSParserContext&, StyleSheetContents*, AllowedRulesType);
        static void parseStyleSheet(const String&, const CSSParserContext&, StyleSheetContents*, CSSParser::RuleParsing);
        static CSSSelectorList parsePageSelector(CSSParserTokenRange, StyleSheetContents*);

        static std::unique_ptr<Vector<double>> parseKeyframeKeyList(const String&);

        bool supportsDeclaration(CSSParserTokenRange&);

        static void parseDeclarationListForInspector(const String&, const CSSParserContext&, CSSParserObserver&);
        static void parseStyleSheetForInspector(const String&, const CSSParserContext&, StyleSheetContents*, CSSParserObserver&);

        static Ref<ImmutableStyleProperties> parseDeferredDeclaration(CSSParserTokenRange, const CSSParserContext&, StyleSheetContents*);
        static void parseDeferredRuleList(CSSParserTokenRange, CSSDeferredParser&, Vector<RefPtr<StyleRuleBase>>&);
        static void parseDeferredKeyframeList(CSSParserTokenRange, CSSDeferredParser&, StyleRuleKeyframes&);

        CSSTokenizer* tokenizer() const { return m_tokenizer.get(); };
        CSSDeferredParser* deferredParser() const { return m_deferredParser.get(); }

    private:
        CSSParserImpl(const CSSParserContext&, StyleSheetContents*);
        CSSParserImpl(CSSDeferredParser&);

        enum RuleListType {
            TopLevelRuleList,
            RegularRuleList,
            KeyframesRuleList
        };

        // Returns whether the first encountered rule was valid
        template<typename T>
        bool consumeRuleList(CSSParserTokenRange, RuleListType, T callback);

        // These two functions update the range they're given
        RefPtr<StyleRuleBase> consumeAtRule(CSSParserTokenRange&, AllowedRulesType);
        RefPtr<StyleRuleBase> consumeQualifiedRule(CSSParserTokenRange&, AllowedRulesType);

        static RefPtr<StyleRuleCharset> consumeCharsetRule(CSSParserTokenRange prelude);
        RefPtr<StyleRuleImport> consumeImportRule(CSSParserTokenRange prelude);
        RefPtr<StyleRuleNamespace> consumeNamespaceRule(CSSParserTokenRange prelude);
        RefPtr<StyleRuleMedia> consumeMediaRule(CSSParserTokenRange prelude, CSSParserTokenRange block);
        RefPtr<StyleRuleSupports> consumeSupportsRule(CSSParserTokenRange prelude, CSSParserTokenRange block);
        RefPtr<StyleRuleViewport> consumeViewportRule(CSSParserTokenRange prelude, CSSParserTokenRange block);
        RefPtr<StyleRuleFontFace> consumeFontFaceRule(CSSParserTokenRange prelude, CSSParserTokenRange block);
        RefPtr<StyleRuleKeyframes> consumeKeyframesRule(bool webkitPrefixed, CSSParserTokenRange prelude, CSSParserTokenRange block);
        RefPtr<StyleRulePage> consumePageRule(CSSParserTokenRange prelude, CSSParserTokenRange block);

        // FIXME-NEWPARSER: Support "apply"
        // void consumeApplyRule(CSSParserTokenRange prelude);

        RefPtr<StyleRuleKeyframe> consumeKeyframeStyleRule(CSSParserTokenRange prelude, CSSParserTokenRange block);
        RefPtr<StyleRule> consumeStyleRule(CSSParserTokenRange prelude, CSSParserTokenRange block);

        void consumeDeclarationList(CSSParserTokenRange, StyleRule::Type);
        void consumeDeclaration(CSSParserTokenRange, StyleRule::Type);
        void consumeDeclarationValue(CSSParserTokenRange, CSSPropertyID, bool important, StyleRule::Type);
        void consumeCustomPropertyValue(CSSParserTokenRange, const AtomString& propertyName, bool important);

        static std::unique_ptr<Vector<double>> consumeKeyframeKeyList(CSSParserTokenRange);

        Ref<DeferredStyleProperties> createDeferredStyleProperties(const CSSParserTokenRange& propertyRange);

        void adoptTokenizerEscapedStrings();

        // FIXME: Can we build StylePropertySets directly?
        // FIXME: Investigate using a smaller inline buffer
        ParsedPropertyVector m_parsedProperties;
        const CSSParserContext& m_context;

        RefPtr<StyleSheetContents> m_styleSheet;

        // For deferred property parsing.
        RefPtr<CSSDeferredParser> m_deferredParser;

        // For normal parsing.
        std::unique_ptr<CSSTokenizer> m_tokenizer;

        // For the inspector
        CSSParserObserverWrapper* m_observerWrapper { nullptr };


    };

}
#endif //RUBRIC_CSSPARSERIMPL_H
