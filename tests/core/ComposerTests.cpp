// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <catch2/catch.hpp>
#include <ui/Composer.h>
#include <meta.pb.h>
#include <iostream>

TEST_CASE( "Load Composer Files", "[Composer]" ) {

    rubric::proto::Meta meta;
    auto * object = meta.mutable_parent();
    object->set_type_name("TestObject");
    auto * aProperty = object->add_properties();
    aProperty->set_name("intValue");
    aProperty->set_property_int(32);
    auto data = meta.SerializeAsString();

    SECTION( "deserialize" ) {

        rubric::Composer composer;
        composer.addObjects(data);

    }
}