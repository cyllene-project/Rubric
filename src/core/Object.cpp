//
// Created by bizarro on 6/14/19.
//

#include "Object.h"

using namespace rubric;

Type& Object::getType() {
    return Type::getType<Object>();
}

void Object::setProperty(std::string & propertyName, std::any & propertyValue) {

    auto t = getType();

    auto p = t.getProperty(propertyName);

    auto s = p.getSetter();

    s(std::any(this), propertyValue);

}