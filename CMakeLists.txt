cmake_minimum_required(VERSION 3.19)
project(2D_heat_equation)

set(CMAKE_CXX_STANDARD 20)

add_executable(2D_heat_equation main.cpp properties.h mesh.cpp mesh.cpp mesh.h geometric_operations.hpp)