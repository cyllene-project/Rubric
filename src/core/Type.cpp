// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <Type.h>
#include <typeindex>


using namespace rubric;

std::unordered_map<std::string, std::type_index> Type::nameTable;
std::unordered_map<std::type_index, std::shared_ptr<const Type>> Type::registry;
std::unordered_map<std::type_index, std::vector<const PropertyDef*>> Type::propertyRegistry;


Type::Type(const rubric::Type &other) noexcept:
name(other.name),
properties(other.properties),
propertyCount(other.propertyCount),
_type(other._type),
_parent(other._parent) {}

Type& Type::operator=(const rubric::Type & other) {
    return *this;
}

std::shared_ptr<const Type> & Type::getTypeByName(std::string typeName) noexcept(false) {
    return registry.at(nameTable.at(typeName));
}

std::shared_ptr<const Type> & Type::getType(std::type_index typeIndex) noexcept(false) {
    return registry.at(typeIndex);
}

const std::string Type::getName() const {
    return std::string(name);
}

const std::vector<const PropertyDef*> & Type::getProperties() const {
    return propertyRegistry[std::type_index(this->_type)];
}

const PropertyDef &Type::getProperty(const std::string propName) const {
    for(auto i = 0; i < propertyCount; i++) {
        if(properties[i].getName() == propName) {
            return properties[i];
        }
    }
    throw std::invalid_argument(propName);
}


void Type::registerType(std::shared_ptr<const Type> type) {

    nameTable.emplace(type->name, type->_type);
    registry.emplace(std::type_index(type->_type), type);

    auto parentType = &type->_parent;
    auto null_ptr = &typeid(nullptr);

    std::vector<const PropertyDef*> defs;


    while(parentType != null_ptr) {
        auto parentT = getType(*parentType);
        defs.insert(defs.end(), parentT->getProperties().begin(), parentT->getProperties().end());
        parentType = &parentT->_parent;
    }

    for (auto i=0; i< type->propertyCount; ++i) {
        defs.insert(defs.end(), &type->properties[i]);
    }
    propertyRegistry[std::type_index(type->_type)] = defs;
}

