// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_OBJECT_H
#define RUBRIC_OBJECT_H

#include <string>
#include <map>

#define REGISTER_TYPE(klass) \
    class klass##Factory : public ObjectFactory { \
    public: \
        klass##Factory() { \
            Object::registerType(#klass, this); \
        } \
        virtual Object *create() { \
            return new klass(); \
        } \
    }; \
    static klass##Factory global_##klass##Factory;

namespace rubric {
    class ObjectFactory;
    /**
     * Base class for all Rubric objects
     */
    class Object {

    public:
        static void registerType(const std::string& name, ObjectFactory *factory);
    private:
        static std::map<std::string, ObjectFactory*> factories;

    };

    class ObjectFactory {
    public:
        virtual Object *create() = 0;
    };


}
#endif //RUBRIC_OBJECT_H
