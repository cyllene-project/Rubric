// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "draw/Renderer.h"

using namespace rubric::draw;

std::map<std::string, RendererFactory*> Renderer::renderers;

void Renderer::registerRenderer(const std::string & name, rubric::draw::RendererFactory * factory) noexcept {
    renderers[name] = factory;
}

std::map<std::string, RendererFactory*> Renderer::getRenderers() noexcept {
    return renderers;
}
