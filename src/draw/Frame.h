// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_PAINTER_H
#define RUBRIC_PAINTER_H

#include "RenderNode.h"
#include "Transform.h"
#include <vector>
#include <memory>
#include <any>

namespace rubric::draw {

    class Frame {

    public:
        explicit Frame();

        //std::shared_ptr<RenderNode> getNode();
        //std::unique_ptr<RenderNode> toNode();

        //void transform(Matrix);
        //void transform(Point);



    private:

        struct State {
            int startIndex;
            int numNodes;
            Transform transformation;
            std::any data;
        };

        Frame::State & getCurrentState();

        void pushState();

        std::vector<State> states;
        std::vector<std::unique_ptr<RenderNode>> node;

    };

}

#endif //RUBRIC_PAINTER_H
