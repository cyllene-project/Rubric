cmake_minimum_required(VERSION 3.13)
project(Rubric)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

include(cmake/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(${Rubric_SOURCE_DIR}/include)
include_directories(${Rubric_SOURCE_DIR}/src)

find_package(Wayland REQUIRED)
find_package(X11_XCB REQUIRED)
find_package(rxcpp REQUIRED)
find_package(Protobuf REQUIRED)
find_package(xpp REQUIRED)
find_package(bgfx REQUIRED)
find_package(kiwi REQUIRED)

include_directories(${WAYLAND_CLIENT_INCLUDE_DIR})

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

add_library(Rubric SHARED
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
        include/core/PropertySpecializations.h src/core/Event.cpp include/core/Event.h)

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
        src/draw/Region.h src/draw/blend2d/Blend2DRenderer.cpp src/draw/blend2d/Blend2DRenderer.h src/draw/X11/X11EventHandler.h src/draw/css/parser/CSSParser.cpp src/draw/css/parser/CSSParser.h src/draw/css/parser/CSSParserContext.cpp src/draw/css/parser/CSSParserContext.h)

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

include(CTest)
include(Catch)

catch_discover_tests(tests)
