cmake_minimum_required(VERSION 3.21)
project(Refactoring)

set(CMAKE_CXX_STANDARD 20)

add_executable(Refactoring refactoring.cpp json.cpp json.h xml.cpp xml.h)
