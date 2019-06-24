//
// Created by bizarro on 6/14/19.
//

#ifndef RUBRIC_WINDOW_H
#define RUBRIC_WINDOW_H

#include "core/Object.h"
#include "ui/View.h"


namespace rubric::ui {

    enum class WindowType { topLevel };

    class Window: public View {

    public:
        explicit Window(WindowType = WindowType::topLevel);
        void addSubView(View &) override;
        void show() override;
        void hide() override;
        void realize() override;
        std::shared_ptr<const Type> & getType() const override;


    private:
        bool clientDecorated = true;
    };
}

#endif //RUBRIC_WINDOW_H
