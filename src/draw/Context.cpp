// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <iostream>
#include <draw/Context.h>
#include <draw/Display.h>
#include <draw/DisplayManager.h>

using namespace rubric::core;

namespace rubric::draw {

    struct Context::ContextImpl {

        explicit ContextImpl(Context & ctxt):
        displayManager(ctxt){ }

        DisplayManager displayManager;
        MainLoop runLoop;
    };


    Context Context::instance;

    Context & Context::getInstance() {
        return instance;
    }

    Context::Context() noexcept :
    impl(std::make_unique<ContextImpl>(*this)) {

        auto display = impl->displayManager.getDefaultDisplay();

        if (!display) {
            impl->displayManager.openDisplay("");
        }
    }

    std::shared_ptr<Display> Context::getDefaultDisplay() const {
        return impl->displayManager.getDefaultDisplay();
    }


    MainLoop & Context::getRunLoop() {
        return impl->runLoop;
    }
}