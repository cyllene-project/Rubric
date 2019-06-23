// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_VIEW_H
#define RUBRIC_VIEW_H

#include <memory>
#include "Object.h"

namespace rubric::ui {

    class View: public Object {

    public:

        virtual void addSubView(View &) = 0;
        virtual void show() = 0;
        virtual void hide() = 0;
        virtual void realize() = 0;

        bool isVisible() const;
        void setVisible(bool);
        std::weak_ptr<View> getParent() const;
        std::shared_ptr<const Type> & getType() const override;
    private:
        bool visible = false;
        std::weak_ptr<View> parent;
    };

}
#endif //RUBRIC_VIEW_H
