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
                surface = rubric::ui::Context::getInstance()
                        .getDisplayManager()
                        .getDefaultDisplay()
                        ->createSurface(
                            SurfaceType::topLevel, 0, 0, 0, 0);
                break;

            default:
                break;
        }

        if (!renderer) {

        }

    }

    void Window::render(rubric::draw::Surface &) {

    }
}