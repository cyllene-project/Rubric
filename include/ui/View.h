// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_VIEW_H
#define RUBRIC_VIEW_H

#include <memory>
#include <draw/Surface.h>
#include <core/Property.h>
#include <draw/Painter.h>
#include "core/Object.h"
#include "LayoutManager.h"

namespace rubric::ui {


    class View: public Object {

    public:
        // Abstract methods to be implemented by each subclass
        virtual void addSubView(View &) = 0;
        virtual void show() = 0;
        virtual void hide() = 0;
        virtual void realize() = 0;
        virtual void render(rubric::draw::Painter &) = 0;
        // Virtual methods that can be overriden by subclasses
        virtual void render(rubric::draw::Surface &);
        // Base implementations
        void addLayoutManager(std::unique_ptr<LayoutManager>);
        std::weak_ptr<View> getParent() const;
        std::shared_ptr<const Type> & getType() const override;
        // Properties
        Property<bool> visible { false };

    protected:
        std::weak_ptr<View> parent;
        std::unique_ptr<LayoutManager> layoutManager;

    };

}
#endif //RUBRIC_VIEW_H
