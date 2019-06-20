// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <Type.h>
#include <typeindex>


using namespace rubric;


std::unordered_map<std::string, std::type_index> Type::nameTable;
std::unordered_map<std::type_index, Type> Type::registry;

Type::Type(std::string & typeName) noexcept :
name(typeName) {
}

Type& Type::getTypeByName(std::string typeName) noexcept(false) {
    return registry.at(nameTable.at(typeName));
}

Type& Type::getType(std::type_index typeIndex) noexcept(false) {
    return registry.at(typeIndex);
}

const std::string & Type::getName() const {
    return name;
}

std::vector<PropertyDef> &Type::getProperties() {
    return properties;
}

void Type::addProperty(std::string propName, PropertyDef::Getter propGetter) {
    properties.emplace_back(propName, propGetter);
}

void Type::addProperty(std::string propName, PropertyDef::Getter propGetter, PropertyDef::Setter propSetter) {
    properties.emplace_back(propName, propGetter, propSetter);
}

PropertyDef &Type::getProperty(std::string & propName) {
    for(auto & p : properties) {
        if(p.getName() == propName) {
            return p;
        }
    }
    throw std::invalid_argument(propName);
}
