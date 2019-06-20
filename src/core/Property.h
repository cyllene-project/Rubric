// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_PROPERTY_H
#define RUBRIC_PROPERTY_H

#include <any>

namespace rubric {

    enum class PropertyType { readWrite, readOnly, construct };

    template<typename T>
    class Property {
        T data;

    public:
        Property() : data() { }

        T operator()() const {
            return data;
        }

        T operator()(T const & value) {
            data = value;
            return data;
        }

        // access with get()/set() syntax
        T get() const {
            return data;
        }

        T set(T const & value) {
            data = value;
            return data;
        }

        // access with '=' sign
        operator T() const {
            return data;
        }

        T& operator=(T const & value) {
            data = value;
            return data;
        }

        T& operator=(const std::any & value) {
            data = std::any_cast<T>(value);
            return data;
        }

    private:
        PropertyType propertyType;
    };

}

#endif //RUBRIC_PROPERTY_H
