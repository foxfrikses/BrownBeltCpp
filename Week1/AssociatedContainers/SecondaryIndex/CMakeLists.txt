cmake_minimum_required(VERSION 3.21)
project(SecondaryIndex)

set(CMAKE_CXX_STANDARD 20)

add_executable(SecondaryIndex secondary_index.cpp database.h)
