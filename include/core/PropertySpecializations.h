// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_PROPERTYSPECIALIZATIONS_H
#define RUBRIC_PROPERTYSPECIALIZATIONS_H

#include <any>

namespace rubric {

    template<class T>
    class Property;

    template <>
    class Property<std::string> {
        std::string data;
    public:
        void set(std::string const &);
        std::string get() const;
        std::string operator=(char const*);
        std::string operator=(const std::any &);
        bool operator==(const std::string) const;
    };

    template <>
    class Property<bool> {
        bool data;
    public:
        explicit Property(bool);

        bool set(const bool &);
        bool get() const;
        bool operator=(const std::any &);
        bool operator=(const bool);
        bool operator==(const bool);
        bool operator()(const bool &);
        bool operator!=(const bool) const;
        bool operator!();
        bool operator()() const;
        operator bool() const;
    };

    template <>
    class Property<int> {
        int data;
    public:
        Property();
        explicit Property(int);
        int set(const int &);
        int get() const;
        int operator=(const std::any &);
        int operator=(const int);
        bool operator==(const int) const;
    };

}

#endif //RUBRIC_PROPERTYSPECIALIZATIONS_H
