cmake_minimum_required(VERSION 3.29)
project(CSC8501Coursework)

set(CMAKE_CXX_STANDARD 20)

add_executable(CSC8501Coursework main.cpp
        memoryAllocation.cpp
        FastestSorter.cpp
        PointersAndReferences.cpp
        passing_parameters.cpp
        interaction_and_recursion.cpp
        arrays.cpp
        ConwaysGameOfLife/main.cpp
        ConwaysGameOfLife/grid.h
        ConwaysGameOfLife/grid.tpp
)
