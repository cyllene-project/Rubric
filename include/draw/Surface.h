// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_SURFACE_H
#define RUBRIC_SURFACE_H

#include <memory>
#include <rxcpp/rx.hpp>
#include <core/Event.h>
#include "draw/Region.h"

namespace rubric::draw {

    class Display;

    enum class SurfaceType { topLevel, menu, popup };


    class Surface {

    public:
        virtual Display & getDisplay() = 0;
        virtual void show();
        virtual void hide();
        virtual void resize(int, int);
        virtual void setTitle(const std::string &) = 0;

        virtual void tick() const;

        rxcpp::observable<Region> renderUpdates();

        rxcpp::observable<Event> eventStream();

        rxcpp::observable<std::shared_ptr<Surface>> sizeUpdates();




    protected:

        rxcpp::subjects::subject<Region> render;

        rxcpp::subjects::subject<Event> events;

        rxcpp::subjects::subject<std::shared_ptr<Surface>> sizeChanged;

        std::shared_ptr<Surface> parent;

    };

}
#endif //RUBRIC_SURFACE_H
