// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <Rubric.h>

using namespace rubric;

RUBRIC_REGISTER_TYPE(Object, nullptr)

void Object::setProperty(const std::string propertyName, std::any & propertyValue) {
    const auto l = getType()->getProperty(propertyName).getSetter();
    l(std::any(this), propertyValue);
}

std::any Object::getProperty(const std::string propertyName) {
    const auto s = getType()->getProperty(propertyName).getGetter();
    return s(std::any(this));
}

