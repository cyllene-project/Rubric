// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_X11EVENTHANDLER_H
#define RUBRIC_X11EVENTHANDLER_H

#include <xpp/xpp.hpp>
#include <X11/Xlib.h> // XKeysymToString
#include <X11/keysym.h> // XK_Escape
#include <typeindex>
#include <any>

namespace rubric::draw::x11 {

    namespace x {
        using connection = xpp::connection<>;
        using registry = xpp::event::registry<connection &>;
        using window = xpp::window<connection &>;

        using key_press = xpp::x::event::key_press<connection &>;
        using key_release = xpp::x::event::key_release<connection &>;
        using button_press = xpp::x::event::button_press<connection &>;
        using expose = xpp::x::event::expose<connection &>;
    }


    struct Event {
        std::any event;
        const std::type_index typeInfo;

        template<typename E>
        operator E() const {
            return std::any_cast<E>(event);
        }

        template<typename E>
        bool eventType() {
            return typeInfo == std::type_index(typeid(E));
        }
    };


    template<typename Connection, typename ... Extensions>
    class EventHandler :
            public xpp::x::event::dispatcher<Connection>
            , public Extensions::template event_dispatcher<Connection> ... {


    public:
        template<typename C>
        explicit
        EventHandler(C && c)
        : xpp::x::event::dispatcher<Connection>(std::forward<C>(c))
        , Extensions::template event_dispatcher<Connection>(
                std::forward<C>(c), c.template extension<Extensions>()) ...{}

        void handle(const std::shared_ptr<xcb_generic_event_t> & event) const {
            handle<xpp::x::extension, Extensions ...>(event);
        }

        rxcpp::observable<Event> eventStream() {
            return events.get_observable();
        }

        template<typename E>
        rxcpp::observable<E> eventStream() {
            return events
            .get_observable()
            .filter([](Event e) {
                return e.eventType<E>();
            });
        }

        rxcpp::subjects::subject<Event> events;

    private:


        struct handler {
            explicit handler(const EventHandler<Connection, Extensions ...> & registry)
                    : m_registry(registry)
            {}

            const EventHandler<Connection, Extensions ...> & m_registry;

            template<typename Event>
            void operator()(const Event & event) const {
                m_registry
                .events
                .get_subscriber()
                .on_next(rubric::draw::x11::Event { event, typeid(decltype(event)) });
            }
        };

        template<typename Extension>
        bool handle(const std::shared_ptr<xcb_generic_event_t> & event) const {
            typedef const typename Extension::template event_dispatcher<Connection> & dispatcher;
            return static_cast<dispatcher>(*this)(handler(*this), event);
        }

    };


}


#endif //RUBRIC_X11EVENTHANDLER_H
