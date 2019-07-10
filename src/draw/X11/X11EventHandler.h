// Rubric
// Copyright (c) 2019 Chris Daley <chebizarro@gmail.com>
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef RUBRIC_X11EVENTHANDLER_H
#define RUBRIC_X11EVENTHANDLER_H

#include <xpp/xpp.hpp>
#include <X11/Xlib.h> // XKeysymToString
#include <X11/keysym.h> // XK_Escape

namespace rubric::draw::x11 {

    namespace x {
        using connection = xpp::connection<>;

        using window = xpp::window<connection &>;

        using key_press = xpp::x::event::key_press<connection &>;
        using key_release = xpp::x::event::key_release<connection &>;
        using button_press = xpp::x::event::button_press<connection &>;
    }

    template<typename Connection>
    class EventHandler :
            public xpp::event::sink<x::key_press, x::key_release, x::button_press> {

    public:
        template <typename C>
        explicit EventHandler(C && c):
        m_c(std::forward<C>(c)) {}

        void handle(const x::key_press & e) override {
            auto kbd_mapping = m_c.get_keyboard_mapping(e->detail, 1);
            // take the first value from the kbd_mapping list
            // This might throw, but for simplicity, no error handling here
            auto keysym = *kbd_mapping.keysyms().begin();

            if (keysym == XK_Escape) {
                std::cerr << "quitting" << std::endl;
                // parameter has a default value: XCB_TIME_CURRENT_TIME
                m_c.ungrab_keyboard();
                //g_quit = true;
            } else {
                std::cerr << "key press: " << XKeysymToString(keysym) << std::endl;
            }
        }

        // xpp::event::sink<x::key_release>::handle(...) interface
        void handle(const x::key_release & e) override {
            auto kbd_mapping = m_c.get_keyboard_mapping(e->detail, 1);
            auto keysym = *kbd_mapping.keysyms().begin();
            std::cerr << "key release: " << XKeysymToString(keysym) << std::endl;
        }

        // xpp::event::sink<x::button_press>::handle(...) interface
        void handle(const x::button_press & e) override {
            m_c.ungrab_pointer(XCB_TIME_CURRENT_TIME);

            // event & reply accessors have a default template parameter, the c-type
            // Usable with any type which is constructible from the c-type or
            // connection + c-type
            // xcb_window_t grab_window = e.event();
            x::window grab_window = e.event<x::window>();

            if (e->event == e->root) {
                // xpp::window, etc. are assignable with the c-type
                grab_window = e.child();
                // xpp::window, etc. are implicitly convertible to c-type
                auto translate = grab_window.translate_coordinates(grab_window, 1, 1);
                grab_window = translate->child;
            }

            *m_c.grab_keyboard(true, grab_window,
                               XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);

            std::cerr << "Grabbed " << grab_window
                      << ". Press Escape to quit." << std::endl;
        }



    private:
        Connection m_c;

    };


}


#endif //RUBRIC_X11EVENTHANDLER_H
