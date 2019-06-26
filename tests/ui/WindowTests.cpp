// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <catch2/catch.hpp>
#include <ui/Window.h>

TEST_CASE( "Windows can be instantiated and shown", "[Window]" ) {

    rubric::ui::Window window;

    window.title = "Test Window";

    REQUIRE( !window.visible );

    SECTION( "show window" ) {
        window.show();

        REQUIRE( window.visible );
    }
}
