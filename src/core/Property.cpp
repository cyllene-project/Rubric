// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <string>
#include "core/PropertySpecializations.h"


std::string rubric::Property<std::string>::operator=(char const* value) {
    return set(value);
}

std::string rubric::Property<std::string>::get() const {
    return data;
}

std::string rubric::Property<std::string>::set(std::string const & value) {
    data = value;
}

std::string rubric::Property<std::string>::operator=(const std::any & value) {
    return set(std::any_cast<std::string>(value));
}

bool rubric::Property<std::string>::operator==(const std::string other) const {
    return (data == other);
}


rubric::Property<bool>::Property(bool value): data(value){}

rubric::Property<bool>::operator bool() const {
    return data;
}

bool rubric::Property<bool>::operator!() {
    return !data;
}

bool rubric::Property<bool>::operator==(const bool other) {
    return (data == other);
}

bool rubric::Property<bool>::operator=(const bool value) {
    return set(value);
}

bool rubric::Property<bool>::get() const {
    return data;
}

bool rubric::Property<bool>::operator=(const std::any & value) {
    return set(std::any_cast<bool>(value));
}

bool rubric::Property<bool>::set(const bool & value) {
    data = value;
    return data;
}


rubric::Property<int>::Property() {}

rubric::Property<int>::Property(int value): data(value) {}

int rubric::Property<int>::operator=(const int value) {
    return set(value);
}

int rubric::Property<int>::get() const {
    return data;
}

int rubric::Property<int>::operator=(const std::any & value) {
    return set(std::any_cast<int>(value));
}

bool rubric::Property<int>::operator==(const int other) const {
    return data == other;
}

int rubric::Property<int>::set(const int & value) {
    data = value;
    return data;
}