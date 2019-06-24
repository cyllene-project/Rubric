// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <catch2/catch.hpp>
#include <core/Property.h>

using namespace rubric;

TEST_CASE( "Create and manipulate Properties", "[Properties]" ) {

    Property<std::string> testProp;

    SECTION( "set property") {
        testProp = std::string("testValue");
        REQUIRE( testProp == std::string("testValue") );
    }

    SECTION( "get property") {
        testProp = std::string("testValue");
        const std::string testVal = testProp;
        REQUIRE( testVal == std::string("testValue") );
    }


}
