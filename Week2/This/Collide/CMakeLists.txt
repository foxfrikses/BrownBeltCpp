cmake_minimum_required(VERSION 3.21)
project(Collide)

set(CMAKE_CXX_STANDARD 20)

add_executable(Collide collide.cpp geo2d.cpp geo2d.h game_object.h)
