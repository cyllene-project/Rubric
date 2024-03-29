// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_TYPE_H
#define RUBRIC_TYPE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <typeindex>
#include <utility>
#include <iostream>
#include <core/PropertyDef.h>

/**
 * RUBRIC_REGISTER_TYPE
 * usage:
 *
 * With getters and setters
 * RUBRIC_REGISTER_TYPE(MyClass, Object, RUBRIC_PROPERTY(name))
 *
 * With getter only
 * RUBRIC_REGISTER_TYPE(MyClass, Object, RUBRIC_PROPERTY_RO(name))
 *
 * Multiple properties
 * RUBRIC_REGISTER_TYPE(MyClass, Object, \
 *  RUBRIC_PROPERTY(name), \
 *  RUBRIC_PROPERTY(synced)\
 * )
 */

#define RUBRIC_TYPE_FACTORY(klass, parent, factory, ...) \
class klass##Factory : public rubric::factory##Factory<klass> { \
public: \
    klass##Factory() noexcept { \
        using type = klass; \
        static constexpr const rubric::PropertyDef properties[] = { \
            __VA_ARGS__ \
        }; \
        static const int propCount = sizeof(properties)/sizeof(rubric::PropertyDef); \
        static constexpr const rubric::Type thisType(#klass, typeid(klass), typeid(parent), properties, propCount); \
        static const auto typePtr = std::shared_ptr<const rubric::Type>(&thisType, [](const rubric::Type* ptr){} ); \
        rubric::Type::registerType(typePtr); \
    } \
}; \
static klass##Factory global_##klass##Factory; \
std::shared_ptr<const rubric::Type> & klass::getType() const {\
    return rubric::Type::getType<klass>();\
}

#define RUBRIC_REGISTER_TYPE(klass, parent, ...) \
RUBRIC_TYPE_FACTORY(klass, parent, Type, __VA_ARGS__)

#define RUBRIC_REGISTER_ABSTRACT_TYPE(klass, parent, ...) \
RUBRIC_TYPE_FACTORY(klass, parent, AbstractType, __VA_ARGS__)

#define RUBRIC_PROPERTY(name) \
rubric::PropertyDef { \
    #name, \
    defaultGetter<decltype(type::name), &type::name>(), \
    defaultSetter<decltype(type::name), &type::name>() \
}

#define RUBRIC_PROPERTY_RO(name) \
rubric::PropertyDef { #name, defaultGetter<decltype(type::name), &type::name>() }


namespace rubric {

    class Object;

    class BaseFactory { };

    template <typename T>
    class AbstractTypeFactory : public BaseFactory {

    public:
        virtual std::shared_ptr<T> create() {
            throw std::invalid_argument("Cannot instantiate Abstract Type");
        }

        template<class P, P T::*name>
        static constexpr const PropertyDef::Getter defaultGetter() {
            return (rubric::PropertyDef::Getter) [](std::any obj) {
                auto *o = dynamic_cast<T*>(std::any_cast<Object*>(obj));
                auto p = o->*name;
                return std::any(p.get());
            };
        }

        template<class P, P T::*name>
        static constexpr const PropertyDef::Setter defaultSetter() {
            return (rubric::PropertyDef::Setter) [](std::any obj, std::any val) {
                T *o = dynamic_cast<T*>(std::any_cast<Object*>(obj));
                o->*name = val;
            };
        }

    };

    template<typename T>
    class TypeFactory : public AbstractTypeFactory<T> {
    public:
        std::shared_ptr<T> create() override {
            return std::make_shared<T>();
        }
    };


    class Type {

    public:

        constexpr Type(std::string_view name,
                       const std::type_info &typeIndex,
                       const std::type_info &parentType,
                       const rubric::PropertyDef *props,
                       int propCount) noexcept:
                name(name),
                properties(props),
                propertyCount(propCount),
                _type(typeIndex),
                _parent(parentType) {}

        Type(const Type&) noexcept;

        Type& operator=(const rubric::Type&);

        const std::string getName() const;

        const std::vector<const PropertyDef*> & getProperties() const;

        const PropertyDef &getProperty(std::string) const;

        static void registerType(std::shared_ptr<const Type> type);

        template<typename T>
        static std::shared_ptr<const Type> registerType(std::string name, BaseFactory *factory) {
            auto id = std::type_index(typeid(T));
            nameTable.emplace(name, id);
            auto t = registry.emplace(id, name);
            return t.first->second;
        }

        static std::shared_ptr<const Type> & getTypeByName(std::string) noexcept(false);

        static std::shared_ptr<const Type> & getType(std::type_index) noexcept(false);

        template<typename T>
        static std::shared_ptr<const Type> & getType() noexcept(false) {
            return getType(std::type_index(typeid(T)));
        }

    private:
        const std::string_view name;
        const PropertyDef *properties;
        const int propertyCount;

        const std::type_info &_type;
        const std::type_info &_parent;

        static std::unordered_map<std::string, std::type_index> nameTable;
        static std::unordered_map<std::type_index, std::shared_ptr<const Type>> registry;
        static std::unordered_map<std::type_index, std::vector<const PropertyDef*>> propertyRegistry;

    };


}
#endif //RUBRIC_TYPE_H
