// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_PROPERTY_H
#define RUBRIC_PROPERTY_H

#include <any>
#include <iostream>
#include <rxcpp/rx.hpp>
#include "core/PropertySpecializations.h"

namespace rubric {

    enum class PropertyType { readWrite, readOnly, empty, construct };

    template<typename T>
    class Property {
        T data;

    public:

        using type = T;

        Property() : data(), valueChanged(data) { }

        explicit Property(T value) : data(value), valueChanged(value) { }

        T operator()() const {
            return data;
        }

        T operator()(T const & value) {
            return set(value);
        }

        // access with get()/set() syntax
        T get() const {
            return data;
        }

        T set(T const & value) {
            data = std::move(value);
            valueChanged.get_subscriber().on_next(data);
            return data;
        }

        // access with '=' sign
        operator T() const {
            return data;
        }

        /*
        T& operator=(const T & value) {
            return set(value);
        }*/

        T operator=(const std::any & value) {
            return set(std::any_cast<T>(value));
        }

        /*
        T operator=(char const * value) {
            return set(value);
        }*/

        T operator=(const T value) {
            return set(value);
        }

        bool operator==(const T other) const {
            return (data == other);
        }

        bool operator!=(const T other) const {
            return (data != other);
        }

        bool operator!() {
            return !data;
        }

        auto subscribe() {
            return valueChanged;
        }

    private:
        rxcpp::subjects::behavior<T> valueChanged;
    };
}

#endif //RUBRIC_PROPERTY_H
