// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_REF_PTR_H
#define RUBRIC_REF_PTR_H

#include <memory>

namespace rubric::draw::css {

    template <typename T>
    class ref_ptr {

    public:
        typedef T type;

        template<typename... Args>
        explicit ref_ptr(Args&&... args) :
        _ptr(std::forward(args)...) {}

        ref_ptr& operator=(const ref_ptr& x) noexcept = default;

        operator T& () const noexcept { return *_ptr; }
        T& get() const noexcept { return *_ptr; }

        T* ptr() const {
            return _ptr.get();
        }

    private:
        std::shared_ptr<T> _ptr;

    };
}

#endif //RUBRIC_REF_PTR_H
