cmake_minimum_required(VERSION 3.21)
project(formal_language)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
add_subdirectory(lr)
add_subdirectory(lr_tests)
add_executable(LR main.cpp)

find_package(GTest)