// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_PROPERTYDEF_H
#define RUBRIC_PROPERTYDEF_H

#include <string>
#include <any>
#include "Property.h"

namespace rubric {

    class PropertyDef {

    public:
        using Getter = std::any (*)(std::any);
        using Setter = void (*)(std::any, std::any);

        PropertyDef(std::string &,
                Getter,
                Setter) noexcept;

        PropertyDef(std::string &,
                    Getter) noexcept;

        const std::string& getName() const noexcept {
            return name;
        }

        Getter getGetter() noexcept;
        Setter getSetter() noexcept;

    private:
        const std::string name;
        PropertyType type;
        Getter getter;
        Setter setter;
    };
}

#endif //RUBRIC_PROPERTYDEF_H
