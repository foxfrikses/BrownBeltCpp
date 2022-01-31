cmake_minimum_required(VERSION 3.21)
project(Texture)

set(CMAKE_CXX_STANDARD 20)

add_executable(Texture main.cpp Textures.cpp Textures.h Common.h Solution.cpp)
