// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_TYPE_H
#define RUBRIC_TYPE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "core/PropertyDef.h"
#include <typeindex>

/**
 * REGISTER_TYPE
 * usage:
 *
 * With getters and setters
 * REGISTER_TYPE(MyClass, Object, PROPERTY(name, name, std::string))
 *
 * With getter only
 * REGISTER_TYPE(MyClass, Object, PROPERTY_RO(name, name, std::string))
 *
 * With custom getter/setter
 * REGISTER_TYPE(MyClass, Object, PROPERTY_CUSTOM(name, std::string, MyClass::getMyName, MyClass::setMyName))
 *
 * Multiple properties
 * REGISTER_TYPE(MyClass, Object, \
 *  PROPERTY(name, name, std::string), \
 *  PROPERTY(synced, synced, bool)\
 * )
 */

#define RUBRIC_REGISTER_TYPE(klass, parent, ...) \
class klass##Factory : public rubric::TypeFactory<klass> { \
    public: \
        klass##Factory() noexcept { \
            auto & t = rubric::Type::registerType<klass>(#klass, this); \
            {\
            __VA_ARGS__;\
            }\
        } \
        virtual std::shared_ptr<klass> create() { \
            return std::make_shared<klass>(); \
        } \
    }; \
    static klass##Factory global_##klass##Factory;

#define RUBRIC_PROPERTY(klass, name) \
    t.addProperty(#name, \
        (rubric::PropertyDef::Getter)[](std::any obj) { \
            auto c = std::any_cast<klass*>(obj); \
            return std::any(c->name); \
        }, \
        (rubric::PropertyDef::Setter)[](std::any obj, std::any val) { \
            auto c = std::any_cast<klass*>(obj); \
            c->name = val; \
        });


namespace rubric {

    template <typename T>
    class TypeFactory {
    public:
        virtual std::shared_ptr<T> create() {
            return std::make_shared<T>();
        }
    };

    class Type {

    public:
        explicit Type(std::string &) noexcept;
        const std::string & getName() const;

        void addProperty(std::string, PropertyDef::Getter, PropertyDef::Setter);

        void addProperty(std::string, PropertyDef::Getter);

        std::vector<PropertyDef> & getProperties();

        PropertyDef & getProperty(std::string &);

        template <typename T>
        static Type & registerType(std::string name, TypeFactory<T> *factory) {
            auto id = std::type_index(typeid(T));
            nameTable.emplace(name, id);
            auto t = registry.emplace(id, name);
            return t.first->second;
        }

        static Type & getTypeByName(std::string) noexcept(false);

        static Type & getType(std::type_index) noexcept(false);

        template <typename T>
        static Type & getType() noexcept(false) {
            return getType(std::type_index(typeid(T)));
        }



    private:

        const std::string name;

        std::vector<PropertyDef> properties;

        static std::unordered_map<std::string, std::type_index> nameTable;
        static std::unordered_map<std::type_index, Type> registry;

    };



}
#endif //RUBRIC_TYPE_H
