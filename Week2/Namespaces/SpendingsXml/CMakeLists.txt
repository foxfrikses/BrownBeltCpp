cmake_minimum_required(VERSION 3.21)
project(SpendingsXml)

set(CMAKE_CXX_STANDARD 20)

add_executable(SpendingsXml spendings_xml.cpp xml.cpp)
