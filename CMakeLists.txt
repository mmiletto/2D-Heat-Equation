cmake_minimum_required(VERSION 3.19)
project(2D_heat_equation)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        2D_heat_equation
        source/c++/main.cpp
        source/c++/properties.h
        source/c++/mesh.cpp
        source/c++/mesh.cpp
        source/c++/mesh.h
        source/c++/geometric_operations.hpp)