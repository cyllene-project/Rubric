// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <Rubric.h>
#include "ui/Window.h"
#include <draw/Context.h>

namespace rubric {
    extern template class Property<bool>;
}


namespace rubric::ui {

    RUBRIC_REGISTER_TYPE(Window, View,
        RUBRIC_PROPERTY(title),
        RUBRIC_PROPERTY(clientDecorated)
    )

    Window::Window(WindowType wtype):
    windowType(wtype) {

    }

    void Window::addSubView(rubric::ui::View & subView) {

    }

    void Window::show() {
        realize();
        visible = true;
    }

    void Window::hide() {

    }

    void Window::realize() {

        if (!clientDecorated) {

        }
        switch (windowType) {
            case WindowType::topLevel:
                surface = Context::getInstance()
                        .getDefaultDisplay()
                        ->createSurface(
                                SurfaceType::topLevel, std::shared_ptr<Surface>(), 0, 0, 100, 100);
                break;

            default:
                break;
        }

        surface->onRender()
        .subscribe([this](Region r) {
            //std::cout << "rendering" << std::endl;
            //View::render(*surface, r);
        });

        if (!renderer) {

        }

        if(!title.get().empty()) {

        }

    }

    void Window::draw(rubric::draw::Frame &) {

    }


    std::tuple<int, int> Window::getSurfaceTransform() {
        return std::tuple<int, int>(0,0);
    }
}