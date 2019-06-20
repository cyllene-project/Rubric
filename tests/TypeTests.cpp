// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <catch2/catch.hpp>
#include <Type.h>
#include <Object.h>

class TestObject: public rubric::Object {

public:
    rubric::Property<std::string> test;
    rubric::Property<int> intTest;

};

class TestObjectTypeFactory: public rubric::TypeFactory<TestObject> {

public:
    TestObjectTypeFactory() {
        auto & t = rubric::Type::registerType<TestObject>("TestObject", this);
        t.addProperty("test",
                (rubric::PropertyDef::Getter)[](std::any obj) {
                    auto o = std::any_cast<TestObject*>(obj);
                    return std::any(o->test);
                });
        t.addProperty("intTest",
                (rubric::PropertyDef::Getter)[](std::any obj) {
                    auto o = std::any_cast<TestObject*>(obj);
                    return std::any(o->intTest);
                },
                (rubric::PropertyDef::Setter)[](std::any obj, std::any val) {
                    auto o = std::any_cast<TestObject*>(obj);
                    o->intTest = val;
                });
    }

};

TEST_CASE( "Register Class and Properties", "[Type]" ) {

    TestObjectTypeFactory factory;

    auto t = rubric::Type::getTypeByName("TestObject");

    REQUIRE( t.getName() == "TestObject" );

    SECTION( "get properties" ) {

        auto props = t.getProperties();

        REQUIRE( props.size() == 2 );
        REQUIRE( props[0].getName() == "test");
        REQUIRE( props[1].getName() == "intTest");

    }
}

class TestMacroObject: public rubric::Object {

public:
    rubric::Property<std::string> test;
    rubric::Property<int> intTest;

};

RUBRIC_REGISTER_TYPE(TestMacroObject, rubric::Object, \
    RUBRIC_PROPERTY(TestMacroObject, test) \
    RUBRIC_PROPERTY(TestMacroObject, intTest) )

TEST_CASE( "Register Class and Properties with Macro", "[Type]" ) {

    auto t = rubric::Type::getTypeByName("TestMacroObject");

    REQUIRE( t.getName() == "TestMacroObject" );

    SECTION( "get properties" ) {

        auto props = t.getProperties();

        REQUIRE( props.size() == 2 );
        REQUIRE( props[0].getName() == "test");
        REQUIRE( props[1].getName() == "intTest");

    }
}
