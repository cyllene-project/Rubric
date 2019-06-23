// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <catch2/catch.hpp>
#include <Object.h>

class TestPropertiesObject: public rubric::Object {

public:
    rubric::Property<std::string> test;
    rubric::Property<int> intTest;
    std::shared_ptr<const rubric::Type> & getType() const override;

};

RUBRIC_REGISTER_TYPE(TestPropertiesObject, rubric::Object, \
    RUBRIC_PROPERTY(test), \
    RUBRIC_PROPERTY(intTest) )

TEST_CASE( "Get and Set Properties", "[Object]" ) {

    TestPropertiesObject testObj {};

    SECTION( "get properties" ) {
        auto p = testObj.getProperty("intTest");

        auto intTest = std::any_cast<int>(p);

        REQUIRE( intTest == 0 );
    }

    SECTION( "set properties") {

        testObj.intTest = 99;

        REQUIRE( testObj.intTest == 99 );

        auto val = std::any(10);

        testObj.setProperty("intTest", val);

        REQUIRE( testObj.intTest == 10 );
    }
}
