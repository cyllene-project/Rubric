//
// Created by bizarro on 6/14/19.
//

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

        // Properties
        Property<std::string> title;
        Property<bool> clientDecorated;

    private:
        WindowType windowType;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<Surface> surface;
    };
}

#endif //RUBRIC_WINDOW_H
