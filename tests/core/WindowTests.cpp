// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <catch2/catch.hpp>
#include <ui/Window.h>

TEST_CASE( "Windows can be instantiated and shown", "[Window]" ) {

    rubric::ui::Window window;

    REQUIRE( !window.isVisible() );

    SECTION( "show window" ) {
        window.show();

        REQUIRE( window.isVisible() );
    }
}
