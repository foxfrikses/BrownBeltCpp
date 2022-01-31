cmake_minimum_required(VERSION 3.21)
project(Budget)

set(CMAKE_CXX_STANDARD 20)

add_executable(Budget
        main.cpp
        range.h
        int_type.h
        date.h
        date_period.h
        budget_manager.h
        primitive_budget_manager.h
        query.h
        query.cpp
        query_visitor.h
        query_visitor.cpp
        date.cpp
        date_period.cpp
        primitive_budget_manager.cpp
        segm_tree_budget_manager.h
        segm_tree_budget_manager.cpp
        summing_segment_tree.h
        summing_segment_tree.cpp)
