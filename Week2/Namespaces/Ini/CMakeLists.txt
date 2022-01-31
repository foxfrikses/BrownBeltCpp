cmake_minimum_required(VERSION 3.21)
project(Ini)

set(CMAKE_CXX_STANDARD 20)

add_executable(Ini test_ini.cpp ini.h ini.cpp)
