//
// Created by bizarro on 6/14/19.
//

#include "Object.h"

using namespace rubric;

std::map<std::string, ObjectFactory*> Object::factories;

void Object::registerType(const std::string& name, ObjectFactory *factory) {
    factories[name] = factory;
}