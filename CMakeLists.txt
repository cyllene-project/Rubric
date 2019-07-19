cmake_minimum_required(VERSION 3.13)
project(Rubric)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

include(cmake/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(${Rubric_SOURCE_DIR}/include)
include_directories(${Rubric_SOURCE_DIR}/src)
include_directories(${Rubric_SOURCE_DIR}/src/draw/css)
include_directories(${Rubric_SOURCE_DIR}/src/draw/css/parser)
#include_directories(${Rubric_SOURCE_DIR}/src/draw/webkit)

find_package(Wayland REQUIRED)
find_package(X11_XCB REQUIRED)
find_package(rxcpp REQUIRED)
find_package(Protobuf REQUIRED)
find_package(xpp REQUIRED)
find_package(bgfx REQUIRED)
find_package(kiwi REQUIRED)
find_package(frozen REQUIRED)

# Generate CSS Properties
find_package(Perl REQUIRED)
find_package(Python REQUIRED)

set(CSS_PROPERTY_NAMES ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/CSSProperties.json)
set(CSS_VALUE_KEYWORDS ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/CSSValueKeywords.in ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/SVGCSSValueKeywords.in)
set(CSS_MAKE_VALUES ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/makevalues.pl)
set(CSS_MAKE_PROPS ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/makeprop.pl)

set(CSS_FLAGS
        ENABLE_TEXT_AUTOSIZING
        ENABLE_VARIATION_FONTS
        ENABLE_CURSOR_VISIBILITY
        ENABLE_CSS_IMAGE_ORIENTATION
        ENABLE_CSS_IMAGE_RESOLUTION
        ENABLE_CSS_COMPOSITING
        ENABLE_FILTERS_LEVEL_2
        ENABLE_DARK_MODE_CSS
        ENABLE_CSS3_TEXT
        ENABLE_TOUCH_EVENTS
        ENABLE_POINTER_EVENTS
        )

set(CSS_PROPERTY_NAME_FILES
        CSSPropertyNames.cpp
        CSSPropertyNames.h
        StyleBuilder.cpp
        StylePropertyShorthandFunctions.cpp
        StylePropertyShorthandFunctions.h)

add_custom_command(
        OUTPUT ${CSS_PROPERTY_NAME_FILES}
        COMMAND ${PERL_EXECUTABLE} -pe '' ${CSS_PROPERTY_NAMES} > CSSProperties.json
        COMMAND ${PERL_EXECUTABLE} ${CSS_MAKE_PROPS} --defines ${CSS_FLAGS}
        DEPENDS ${CSS_PROPERTY_NAMES} ${CSS_MAKE_PROPS}
)

set(CSS_VALUE_KEYWORD_FILES
        CSSValueKeywords.h
        CSSValueKeywords.cpp)

add_custom_command(
        OUTPUT ${CSS_VALUE_KEYWORD_FILES}
        COMMAND ${PERL_EXECUTABLE} -pe '' ${CSS_VALUE_KEYWORDS} > CSSValueKeywords.in
        COMMAND ${PERL_EXECUTABLE} ${CSS_MAKE_VALUES} --defines ${CSS_FLAGS}
        DEPENDS ${CSS_VALUE_KEYWORDS} ${CSS_MAKE_VALUES}
)

set(CSS_SELECTOR_SCRIPTS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/makeSelectorPseudoClassAndCompatibilityElementMap.py
        ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/SelectorPseudoClassAndCompatibilityElementMap.in
        )

add_custom_command(
        OUTPUT SelectorPseudoClassAndCompatibilityElementMap.cpp
        COMMAND ${PYTHON_EXECUTABLE} ${CSS_SELECTOR_SCRIPTS} gperf
)

set(CSS_ELEMENT_SCRIPTS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/makeSelectorPseudoElementsMap.py
        ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/SelectorPseudoElementTypeMap.in
        )

add_custom_command(
        OUTPUT SelectorPseudoElementTypeMap.cpp
        COMMAND ${PYTHON_EXECUTABLE} ${CSS_ELEMENT_SCRIPTS} gperf
)

# color names
set(COLOR_GEN ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/make-hash-tools.pl)
set(COLOR_DATA ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/css/ColorData.gperf)
add_custom_command(
        OUTPUT ColorData.cpp
        COMMAND ${PERL_EXECUTABLE} ${COLOR_GEN} . ${COLOR_DATA}
        DEPENDS ${COLOR_GEN} ${COLOR_DATA}
)


include_directories(${WAYLAND_CLIENT_INCLUDE_DIR})

# generate protobuf classes
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS
        proto/object.proto
        proto/property.proto
        proto/meta.proto
        proto/application.proto
        proto/view.proto
        proto/window.proto)

add_library(RubricProto STATIC ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(RubricProto ${PROTOBUF_LIBRARIES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})


# core library
add_library(Rubric SHARED
        CSSProperties.json
        include/core/Type.h
        src/core/Type.cpp
        include/core/Object.h
        src/core/Object.cpp
        src/core/MainLoop.cpp
        include/core/MainLoop.h
        src/core/Property.cpp
        include/core/Property.h
        include/core/PropertyDef.h
        src/core/PropertyDef.cpp
        include/core/PropertySpecializations.h
        src/core/Event.cpp include/core/Event.h src/core/URL.cpp src/core/URL.h)

add_library(RubricDraw SHARED
        src/draw/DisplayManager.cpp
        src/draw/DisplayManager.h
        src/draw/Display.cpp
        include/draw/Display.h
        src/draw/Backend.cpp
        src/draw/Backend.h
        src/draw/wayland/WaylandDisplay.cpp
        src/draw/wayland/WaylandDisplay.h
        src/draw/X11/X11Display.cpp
        src/draw/X11/X11Display.h
        src/draw/wayland/WaylandBackend.cpp
        src/draw/wayland/WaylandBackend.h
        src/draw/X11/X11Backend.cpp
        src/draw/X11/X11Backend.h
        src/draw/Surface.cpp
        include/draw/Surface.h
        src/draw/X11/X11Surface.cpp
        src/draw/X11/X11Surface.h
        src/draw/wayland/WaylandSurface.cpp
        src/draw/wayland/WaylandSurface.h
        src/draw/Renderer.cpp
        include/draw/Renderer.h
        include/draw/Context.h
        src/draw/Context.cpp
        src/draw/text/Context.cpp
        src/draw/text/Context.h
        src/draw/text/Layout.cpp
        src/draw/text/Layout.h
        src/draw/text/GlyphItem.cpp
        src/draw/text/GlyphItem.h
        src/draw/text/Item.cpp
        src/draw/text/Item.h
        src/draw/text/Font.cpp
        src/draw/text/Font.h
        src/draw/RenderNode.cpp
        src/draw/RenderNode.h
        src/draw/Frame.cpp
        include/draw/Frame.h
        src/draw/Transform.cpp
        src/draw/Transform.h
        src/draw/Region.cpp
        src/draw/Region.h
        src/draw/blend2d/Blend2DRenderer.cpp
        src/draw/blend2d/Blend2DRenderer.h
        src/draw/X11/X11EventHandler.h
        src/draw/Image.cpp src/draw/Image.h)

add_library(RubricCSS SHARED
        # css parser
        ${CSS_PROPERTY_NAME_FILES}
        ${CSS_VALUE_KEYWORD_FILES}

        src/draw/css/parser/CSSAtRuleID.cpp
        src/draw/css/parser/CSSAtRuleID.h
        src/draw/css/parser/CSSDeferredParser.cpp
        src/draw/css/parser/CSSDeferredParser.h
        src/draw/css/parser/CSSParser.cpp
        src/draw/css/parser/CSSParser.h
        src/draw/css/parser/CSSParserContext.cpp
        src/draw/css/parser/CSSParserContext.h
        src/draw/css/parser/CSSParserFastPaths.cpp
        src/draw/css/parser/CSSParserFastPaths.h
        src/draw/css/parser/CSSParserIdioms.cpp
        src/draw/css/parser/CSSParserIdioms.h
        src/draw/css/parser/CSSParserImpl.cpp
        src/draw/css/parser/CSSParserImpl.h
        src/draw/css/parser/CSSParserMode.h
        src/draw/css/parser/CSSParserObserver.h
        src/draw/css/parser/CSSParserObserverWrapper.cpp
        src/draw/css/parser/CSSParserObserverWrapper.h
        src/draw/css/parser/CSSParserSelector.cpp
        src/draw/css/parser/CSSParserSelector.h
        src/draw/css/parser/CSSParserToken.cpp
        src/draw/css/parser/CSSParserToken.h
        src/draw/css/parser/CSSParserTokenRange.cpp
        src/draw/css/parser/CSSParserTokenRange.h
        src/draw/css/parser/CSSPropertyParser.cpp
        src/draw/css/parser/CSSPropertyParser.h
        src/draw/css/parser/CSSPropertyParserHelpers.cpp
        src/draw/css/parser/CSSPropertyParserHelpers.h
        src/draw/css/parser/CSSSelectorParser.cpp
        src/draw/css/parser/CSSSelectorParser.h
        src/draw/css/parser/CSSSupportsParser.cpp
        src/draw/css/parser/CSSSupportsParser.h
        src/draw/css/parser/CSSTokenizer.cpp
        src/draw/css/parser/CSSTokenizer.h
        src/draw/css/parser/CSSTokenizerInputStream.cpp
        src/draw/css/parser/CSSTokenizerInputStream.h
        src/draw/css/parser/CSSVariableParser.cpp
        src/draw/css/parser/CSSVariableParser.h
        src/draw/css/parser/MediaQueryBlockWatcher.cpp
        src/draw/css/parser/MediaQueryBlockWatcher.h
        src/draw/css/parser/MediaQueryParser.cpp
        src/draw/css/parser/MediaQueryParser.h
        src/draw/css/parser/SizesAttributeParser.cpp
        src/draw/css/parser/SizesAttributeParser.h
        src/draw/css/parser/SizesCalcParser.cpp
        src/draw/css/parser/SizesCalcParser.h

        # css sources
        SelectorPseudoClassAndCompatibilityElementMap.cpp
        SelectorPseudoElementTypeMap.cpp
        ColorData.cpp

        src/draw/css/BasicShapeFunctions.cpp
        src/draw/css/CSSAspectRatioValue.cpp
        src/draw/css/CSSBasicShapes.cpp
        src/draw/css/CSSBorderImage.cpp
        src/draw/css/CSSBorderImageSliceValue.cpp
        src/draw/css/CSSCalculationValue.cpp
        src/draw/css/CSSCanvasValue.cpp
        src/draw/css/CSSComputedStyleDeclaration.cpp
        src/draw/css/CSSContentDistributionValue.cpp
        src/draw/css/CSSCrossfadeValue.cpp
        src/draw/css/CSSCursorImageValue.cpp
        src/draw/css/CSSCustomIdentValue.cpp
        src/draw/css/CSSCustomPropertyValue.cpp
        src/draw/css/CSSDefaultStyleSheets.cpp
        src/draw/css/CSSFilterImageValue.cpp
        src/draw/css/FontFaceSet.cpp
        src/draw/css/FontFace.cpp
        src/draw/css/CSSFontFace.cpp
        src/draw/css/CSSFontFaceSet.cpp
        src/draw/css/CSSFontFaceRule.cpp
        src/draw/css/CSSFontFaceSource.cpp
        src/draw/css/CSSFontFaceSrcValue.cpp
        src/draw/css/CSSFontFeatureValue.cpp
        src/draw/css/CSSFontSelector.cpp
        src/draw/css/CSSFontStyleRangeValue.cpp
        src/draw/css/CSSFontStyleValue.cpp
        src/draw/css/CSSFontValue.cpp
        src/draw/css/CSSFontVariationValue.cpp
        src/draw/css/CSSFunctionValue.cpp
        src/draw/css/CSSGradientValue.cpp
        src/draw/css/CSSGridAutoRepeatValue.cpp
        src/draw/css/CSSGridIntegerRepeatValue.cpp
        src/draw/css/CSSGridLineNamesValue.cpp
        src/draw/css/CSSGridTemplateAreasValue.cpp
        src/draw/css/CSSGroupingRule.cpp
        src/draw/css/CSSImageGeneratorValue.cpp
        src/draw/css/CSSImageSetValue.cpp
        src/draw/css/CSSImageValue.cpp
        src/draw/css/CSSImportRule.cpp
        src/draw/css/CSSInheritedValue.cpp
        src/draw/css/CSSInitialValue.cpp
        src/draw/css/CSSKeyframeRule.cpp
        src/draw/css/CSSKeyframesRule.cpp
        src/draw/css/CSSLineBoxContainValue.cpp
        src/draw/css/CSSMarkup.cpp
        src/draw/css/CSSMediaRule.cpp
        src/draw/css/CSSNamedImageValue.cpp
        src/draw/css/CSSNamespaceRule.cpp
        src/draw/css/CSSPageRule.cpp
        src/draw/css/CSSPaintImageValue.cpp
        src/draw/css/CSSPendingSubstitutionValue.cpp
        src/draw/css/CSSPrimitiveValue.cpp
        src/draw/css/CSSProperty.cpp
        src/draw/css/CSSPropertySourceData.cpp
        src/draw/css/CSSReflectValue.cpp
        src/draw/css/CSSRegisteredCustomProperty.cpp
        src/draw/css/CSSRevertValue.cpp
        src/draw/css/CSSRule.cpp
        src/draw/css/CSSRuleList.cpp
        src/draw/css/CSSSegmentedFontFace.cpp
        src/draw/css/CSSSelector.cpp
        src/draw/css/CSSSelectorList.cpp
        src/draw/css/CSSShadowValue.cpp
        src/draw/css/CSSStyleDeclaration.cpp
        src/draw/css/CSSStyleRule.cpp
        src/draw/css/CSSStyleSheet.cpp
        src/draw/css/CSSSupportsRule.cpp
        src/draw/css/CSSTimingFunctionValue.cpp
        src/draw/css/CSSToLengthConversionData.cpp
        src/draw/css/CSSToStyleMap.cpp
        src/draw/css/CSSUnicodeRangeValue.cpp
        src/draw/css/CSSUnsetValue.cpp
        src/draw/css/CSSValue.cpp
        src/draw/css/CSSValueList.cpp
        src/draw/css/CSSValuePool.cpp
        src/draw/css/CSSVariableData.cpp
        src/draw/css/CSSVariableReferenceValue.cpp
        src/draw/css/DOMCSSNamespace.cpp
        src/draw/css/DOMCSSRegisterCustomProperty.cpp
        src/draw/css/DOMCSSPaintWorklet.cpp
        src/draw/css/DOMMatrix.cpp
        src/draw/css/DOMMatrixReadOnly.cpp
        src/draw/css/DeprecatedCSSOMPrimitiveValue.cpp
        src/draw/css/DeprecatedCSSOMValue.cpp
        src/draw/css/DeprecatedCSSOMValueList.cpp
        src/draw/css/DocumentRuleSets.cpp
        src/draw/css/ElementRuleCollector.cpp
        src/draw/css/FontVariantBuilder.cpp
        src/draw/css/InspectorCSSOMWrappers.cpp
        src/draw/css/LengthFunctions.cpp
        src/draw/css/MediaFeatureNames.cpp
        src/draw/css/MediaList.cpp
        src/draw/css/MediaQuery.cpp
        src/draw/css/MediaQueryEvaluator.cpp
        src/draw/css/MediaQueryExpression.cpp
        src/draw/css/MediaQueryList.cpp
        src/draw/css/MediaQueryMatcher.cpp
        src/draw/css/MediaQueryParserContext.cpp
        src/draw/css/PageRuleCollector.cpp
        src/draw/css/PropertySetCSSStyleDeclaration.cpp
        src/draw/css/RGBColor.cpp
        src/draw/css/RuleFeature.cpp
        src/draw/css/RuleSet.cpp
        src/draw/css/SVGCSSComputedStyleDeclaration.cpp
        src/draw/css/SelectorChecker.cpp
        src/draw/css/SelectorFilter.cpp
        src/draw/css/StyleColor.cpp
        src/draw/css/StyleMedia.cpp
        src/draw/css/StyleProperties.cpp
        src/draw/css/StylePropertyShorthand.cpp
        src/draw/css/StyleResolver.cpp
        src/draw/css/StyleRule.cpp
        src/draw/css/StyleRuleImport.cpp
        src/draw/css/StyleSheet.cpp
        src/draw/css/StyleSheetContents.cpp
        src/draw/css/StyleSheetList.cpp
        src/draw/css/TransformFunctions.cpp
        src/draw/css/ViewportStyleResolver.cpp
        src/draw/css/WebKitCSSMatrix.cpp
        src/draw/css/WebKitCSSViewportRule.cpp
        src/draw/css/WritingMode.h
        )

add_library(RubricUI SHARED
        src/ui/Application.cpp
        include/ui/Application.h
        src/ui/View.cpp
        include/ui/View.h
        src/ui/Window.cpp
        include/ui/Window.h
        src/ui/Composer.cpp
        include/ui/Composer.h
        src/ui/Control.cpp
        include/ui/Control.h
        src/ui/Container.cpp
        include/ui/Container.h
        src/ui/LayoutManager.cpp
        include/ui/LayoutManager.h
        src/ui/Label.cpp include/ui/Label.h)

set(RUBRIC_LIBS
        RubricUI
        RubricDraw
        Rubric
        RubricProto
        xpp
        -lxcb
        -lxcb-randr
        -lX11
        ${X11_XCB_LIBRARIES}
        ${PROTOBUF_LIBRARIES}
        ${WAYLAND_LIBRARIES})

add_executable(ExampleApp examples/exampleapp.cpp)

target_link_libraries(ExampleApp ${RUBRIC_LIBS})

find_library(LIBOAUTH_LIBRARY NAMES oauth)

set(LIBOAUTH_LIBRARIES ${LIBOAUTH_LIBRARY})
set(LIBOAUTH_INCLUDE_DIRS ${LIBOAUTH_INCLUDE_DIR})

find_package_handle_standard_args(liboauth DEFAULT_MSG
        LIBOAUTH_LIBRARY LIBOAUTH_INCLUDE_DIR)
mark_as_advanced(LIBOAUTH INCLUDE_DIR LIBOAUTH_LIBRARY)

find_package(jsonformoderncpp REQUIRED)
find_package(range-v3 REQUIRED)
find_package(CURL REQUIRED)
include_directories(${jsonformoderncpp_INCLUDE_DIR} ${range-v3_INCLUDE_DIR} ${CURL_INCLUDE_DIR})

add_executable(Twitter
        examples/twitter/main.cpp
        examples/twitter/TwitterApp.cpp
        examples/twitter/TwitterApp.h
        examples/twitter/tweets.cpp examples/twitter/rxcurl.cpp examples/twitter/model.cpp)

target_link_libraries(Twitter ${RUBRIC_LIBS} ${CURL_LIBRARIES} ${LIBOAUTH_LIBRARY})

find_package(Catch2 REQUIRED)

add_executable(tests
        tests/main.cpp
        tests/ui/WindowTests.cpp
        tests/core/ObjectTests.cpp
        tests/core/TypeTests.cpp
        tests/core/PropertyTests.cpp
        tests/ui/ComposerTests.cpp)

target_link_libraries(tests
        Catch2::Catch2
        ${RUBRIC_LIBS})

#include(CTest)
#include(Catch)

#catch_discover_tests(tests)
