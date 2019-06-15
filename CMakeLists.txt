cmake_minimum_required(VERSION 3.13)
project(Rubric)

set(CMAKE_CXX_STANDARD 17)

include_directories(${Rubric_SOURCE_DIR}/include)

add_library(Rubric SHARED
        src/core/Object.cpp
        include/Object.h
        src/ui/Window.cpp
        include/Window.h
        src/core/Application.cpp
        include/Application.h
        src/ui/View.cpp
        include/View.h
        src/draw/DisplayManager.cpp
        include/DisplayManager.h
        src/draw/Display.cpp
        include/Display.h
        src/draw/Backend.cpp
        src/draw/Backend.h
        src/draw/wayland/WaylandDisplay.cpp
        src/draw/wayland/WaylandDisplay.h
        src/main.cpp
        src/draw/X11/X11Display.cpp
        src/draw/X11/X11Display.h
        src/core/Context.cpp
        src/core/Context.h
        src/draw/wayland/WaylandBackend.cpp
        src/draw/wayland/WaylandBackend.h src/draw/X11/X11Backend.cpp src/draw/X11/X11Backend.h)

add_executable(ExampleApp examples/exampleapp.cpp)

