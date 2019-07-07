// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_BACKEND_H
#define RUBRIC_BACKEND_H

#include <draw/Display.h>
#include <memory>
#include <map>
#include <draw/Context.h>

#define RUBRIC_DRAW_REGISTER_BACKEND(klass) \
class klass##Factory : public rubric::draw::BackendFactory { \
    public: \
        klass##Factory() noexcept { \
            rubric::draw::Backend::registerBackend(#klass, this); \
        } \
        virtual std::shared_ptr<rubric::draw::Backend> create() { \
            return std::make_shared<klass>(); \
        } \
    }; \
    static klass##Factory global_##klass##Factory;


namespace rubric::draw {
    class Backend;
    class BackendFactory;
}

class rubric::draw::Backend {
public:
    virtual std::unique_ptr<Display> open(rubric::draw::Context &, const std::string &) const = 0;
    virtual std::string getName() const = 0;
    static void registerBackend(const std::string &, BackendFactory *);
    static std::map<std::string, BackendFactory*> getBackends();
private:
    static std::map<std::string, BackendFactory*> backends;

};

class rubric::draw::BackendFactory {
public:
    virtual  std::shared_ptr<Backend> create() = 0;
};

#endif //RUBRIC_BACKEND_H
