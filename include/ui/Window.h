// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_WINDOW_H
#define RUBRIC_WINDOW_H

#include "core/Object.h"
#include "ui/View.h"
#include <draw/Renderer.h>
#include <core/Property.h>

namespace rubric::ui {

    using namespace rubric::draw;

    enum class WindowType { topLevel };

    class Window: public View {

    public:
        explicit Window(WindowType = WindowType::topLevel);
        void addSubView(View &) override;
        void show() override;
        void hide() override;
        void realize() override;
        std::shared_ptr<const Type> & getType() const override;
        void render(rubric::draw::Surface &) override;
        /**
         * The Window Title.
         * The Window Title as displayed in the Window Title Bar
         **/
        Property<std::string> title;
        Property<bool> clientDecorated { false };

    private:
        WindowType windowType;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<Surface> surface;
    };
}

#endif //RUBRIC_WINDOW_H
