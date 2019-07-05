// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_TRANSFORM_H
#define RUBRIC_TRANSFORM_H

namespace rubric::draw {

    struct Transform {

        enum class Category { unkown, any, _3D, _2D, _2DAffine, _2DTranslate, identity };

        Category category;

       // Transform & next;

    };

}
#endif //RUBRIC_TRANSFORM_H
