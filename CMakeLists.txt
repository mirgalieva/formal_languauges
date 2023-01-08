cmake_minimum_required(VERSION 3.21)
project(formal_language)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
add_subdirectory(earley)
add_subdirectory(earley_test)
add_executable(EarleyParser main.cpp)

find_package(GTest)