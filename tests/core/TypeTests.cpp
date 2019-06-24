// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <catch2/catch.hpp>
#include <core/Type.h>
#include <core/Object.h>

class TestObject: public rubric::Object {

public:
    rubric::Property<std::string> test;
    rubric::Property<int> intTest;

};

class TestObjectTypeFactory: public rubric::TypeFactory<TestObject> {

public:
    TestObjectTypeFactory() {
        static constexpr const rubric::PropertyDef properties[] = {
                rubric::PropertyDef { "test", defaultGetter<decltype(TestObject::test), &TestObject::test>() } ,
                rubric::PropertyDef { "intTest", defaultGetter<decltype(TestObject::intTest), &TestObject::intTest>(), defaultSetter<decltype(TestObject::intTest), &TestObject::intTest>() }
        };
        static const int propCount = sizeof(properties)/sizeof(rubric::PropertyDef);
        static constexpr const rubric::Type thisType("TestObject", typeid(TestObject), typeid(rubric::Object), properties, propCount);
        static const auto typePtr = std::shared_ptr<const rubric::Type>(&thisType, [](const rubric::Type* ptr){} );
        rubric::Type::registerType(typePtr);
        static_assert(properties[0].getPropertyType() == rubric::PropertyType::readOnly);
        static_assert(properties[1].getPropertyType() == rubric::PropertyType::readWrite);
    }
};

TEST_CASE( "Register Class and Properties", "[Type]" ) {

    TestObjectTypeFactory factory;

    auto t = rubric::Type::getTypeByName("TestObject");

    REQUIRE( t->getName() == "TestObject" );

    SECTION( "get properties" ) {

        auto props = t->getProperties();
        REQUIRE( props.size() == 2 );
        REQUIRE( props[0]->getName() == "test");
        REQUIRE( props[1]->getName() == "intTest");
    }
}

class TestMacroObject: public rubric::Object {

public:
    std::shared_ptr<const rubric::Type> & getType() const override;
    rubric::Property<std::string> test;
    rubric::Property<int> intTest;

};

RUBRIC_REGISTER_TYPE(TestMacroObject, rubric::Object, \
    RUBRIC_PROPERTY(test), \
    RUBRIC_PROPERTY(intTest))

TEST_CASE( "Register Class and Properties with Macro", "[Type]" ) {

    auto t = rubric::Type::getTypeByName("TestMacroObject");

    REQUIRE( t->getName() == "TestMacroObject" );

    SECTION( "get properties" ) {

        auto props = t->getProperties();

        REQUIRE( props.size() == 2 );
        REQUIRE( props[0]->getName() == "test");
        REQUIRE( props[1]->getName() == "intTest");

    }
}

class TestInheritanceObject: public TestMacroObject {

public:
    std::shared_ptr<const rubric::Type> & getType() const override;
    rubric::Property<std::string> t3;
    rubric::Property<int> t4;

};

RUBRIC_REGISTER_TYPE(TestInheritanceObject, TestMacroObject, \
    RUBRIC_PROPERTY(t3), \
    RUBRIC_PROPERTY(t4) )


TEST_CASE( "Property inheritance", "[Type]" ) {

    auto t = rubric::Type::getTypeByName("TestInheritanceObject");

    REQUIRE( t->getName() == "TestInheritanceObject" );

    SECTION( "get properties" ) {

        auto props = t->getProperties();

        REQUIRE( props.size() == 4 );
        REQUIRE( props[0]->getName() == "test");
        REQUIRE( props[1]->getName() == "intTest");
        REQUIRE( props[2]->getName() == "t3");
        REQUIRE( props[3]->getName() == "t4");

    }
}
