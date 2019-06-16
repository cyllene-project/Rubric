// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "Backend.h"

using namespace rubric::draw;

std::map<std::string, BackendFactory*> Backend::backends;

void Backend::registerBackend(const std::string& name, BackendFactory *factory) {
    backends[name] = factory;
}

std::map<std::string, BackendFactory*> Backend::getBackends() {
    return backends;
}
