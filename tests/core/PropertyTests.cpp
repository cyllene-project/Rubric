// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <catch2/catch.hpp>
#include <core/Property.h>

using namespace rubric;

TEST_CASE( "Create and manipulate Properties", "[Properties]" ) {

    Property<std::string> testStringProp;

    SECTION( "set string property") {
        testStringProp = "testValue";
        REQUIRE( testStringProp == "testValue" );
    }

    SECTION( "get string property") {
        testStringProp = "testValue";
        REQUIRE( testStringProp == "testValue" );
    }

}
