// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_PROPERTYDEF_H
#define RUBRIC_PROPERTYDEF_H

#include <string>
#include <any>
#include "core/Property.h"

namespace rubric {

    struct PropertyDef {

    public:
        using Getter = std::any (*)(std::any);
        using Setter = void (*)(std::any, std::any);

        constexpr PropertyDef(std::string_view propName,
                              const rubric::PropertyDef::Getter propertyGetter,
                              const rubric::PropertyDef::Setter propertySetter) noexcept:
                name(propName),
                type(PropertyType::readWrite),
                getter(propertyGetter),
                setter(propertySetter) {}

        constexpr PropertyDef(std::string_view propName,
                              const rubric::PropertyDef::Getter propertyGetter) noexcept:
                name(propName),
                type(PropertyType::readOnly),
                getter(propertyGetter),
                setter(nullptr) {}

        constexpr PropertyDef() noexcept:
                name(""),
                type(PropertyType::empty),
                getter(nullptr),
                setter(nullptr) {}

        constexpr PropertyDef(const PropertyDef& other) noexcept = default;

        const std::string getName() const noexcept {
            return std::string(name);
        }

        constexpr Getter getGetter() const noexcept {
            return getter;
        }

        constexpr Setter getSetter() const noexcept {
            return setter;
        }

        constexpr PropertyType getPropertyType() const noexcept {
            return type;
        }

    private:
        const std::string_view name;
        const PropertyType type;
        const Getter getter;
        const Setter setter;
    };
}

#endif //RUBRIC_PROPERTYDEF_H
