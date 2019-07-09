// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_RENDERER_H
#define RUBRIC_RENDERER_H

#include <memory>
#include "draw/Surface.h"


#define RUBRIC_DRAW_REGISTER_RENDERER(klass) \
class klass##Factory : public rubric::draw::RendererFactory { \
    public: \
        klass##Factory() noexcept { \
            rubric::draw::Renderer::registerBackend(#klass, this); \
        } \
        virtual std::shared_ptr<rubric::draw::Renderer> create() { \
            return std::make_shared<klass>(); \
        } \
    }; \
    static klass##Factory global_##klass##Factory;


namespace rubric::draw {

    class Frame;
    class RendererFactory;

    class Renderer {

    public:
        virtual void realize(std::shared_ptr<Surface>) = 0;

        virtual void render(std::shared_ptr<Frame>) = 0;

        static void registerRenderer(const std::string&, RendererFactory *) noexcept;
        static std::map<std::string, RendererFactory*> getRenderers() noexcept;

    private:
        static std::map<std::string, RendererFactory*> renderers;

    };

    class RendererFactory {
    public:
        virtual  std::shared_ptr<Renderer> create() = 0;
    };
}

#endif //RUBRIC_RENDERER_H
