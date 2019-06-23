// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "Composer.h"
#include <meta.pb.h>

void rubric::Composer::addObjects(std::string & data) {

    rubric::proto::Meta meta;

    if (!meta.ParseFromString(data)) {
        return;
    }

    auto parent = meta.parent();

    //parent.type_name();
}
