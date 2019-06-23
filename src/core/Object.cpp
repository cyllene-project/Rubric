//
// Created by bizarro on 6/14/19.
//

#include <Object.h>

using namespace rubric;

RUBRIC_REGISTER_TYPE(Object, nullptr)

void Object::setProperty(const std::string propertyName, std::any & propertyValue) {
    const auto s = getType();
    const auto l = s->getProperty(propertyName).getSetter();
    l(std::any(this), propertyValue);
}

std::any Object::getProperty(const std::string propertyName) {
    const auto s = getType()->getProperty(propertyName).getGetter();
    return s(std::any(this));
}

