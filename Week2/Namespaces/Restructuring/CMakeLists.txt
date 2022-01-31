cmake_minimum_required(VERSION 3.21)
project(Restructuring)

set(CMAKE_CXX_STANDARD 20)

add_executable(Restructuring main.cpp stats_aggregator.cpp stats_aggregator.h stats_aggregator_test.cpp)
