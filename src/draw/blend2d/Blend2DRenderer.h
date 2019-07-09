// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_BLEND2DRENDERER_H
#define RUBRIC_BLEND2DRENDERER_H

#include <draw/Renderer.h>

namespace rubric::draw::blend2d {

    class Blend2DRenderer: public Renderer {

    public:
        void realize(std::shared_ptr<rubric::draw::Surface>) override;
        void render(std::shared_ptr<rubric::draw::Frame>) override;


    };

}

#endif //RUBRIC_BLEND2DRENDERER_H
