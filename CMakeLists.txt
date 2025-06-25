cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PuzzleSolver VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
add_executable(PuzzleSolver
    main.cpp
    solver.cpp
)

# Add optional compiler warnings
target_compile_options(PuzzleSolver PRIVATE -Wall -Wextra -pedantic)
