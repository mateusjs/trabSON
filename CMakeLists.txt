cmake_minimum_required(VERSION 3.10)
project(so)

set(CMAKE_CXX_STANDARD 11)

add_executable(so main.cpp)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
