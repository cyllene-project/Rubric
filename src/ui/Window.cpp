//
// Created by bizarro on 6/14/19.
//

#include <Rubric.h>
#include "ui/Window.h"

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
        setVisible(true);
    }

    void Window::hide() {

    }

    void Window::realize() {

        if (!clientDecorated) {

        }
        switch (windowType) {
            case WindowType::topLevel:

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