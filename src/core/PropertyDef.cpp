// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "PropertyDef.h"

using namespace rubric;

PropertyDef::PropertyDef(std::string & propName,
        rubric::PropertyDef::Getter propertyGetter,
        rubric::PropertyDef::Setter propertySetter) noexcept:
        name(propName),
        type(PropertyType::readWrite),
        getter(propertyGetter),
        setter(propertySetter) { }

PropertyDef::PropertyDef(std::string & propName,
        rubric::PropertyDef::Getter propertyGetter) noexcept:
        name(propName),
        type(PropertyType::readOnly),
        getter(propertyGetter),
        setter(nullptr) { }

PropertyDef::Getter PropertyDef::getGetter() noexcept {
    return getter;
}

PropertyDef::Setter PropertyDef::getSetter() noexcept {
    return setter;
}
