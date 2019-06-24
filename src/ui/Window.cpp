//
// Created by bizarro on 6/14/19.
//

#include <Rubric.h>
#include "ui/Window.h"

namespace rubric::ui {

    RUBRIC_REGISTER_TYPE(Window, View)

    Window::Window(WindowType wtype) {

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


    }
}