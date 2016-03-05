cmake_minimum_required(VERSION 3.3)
project(Threading)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES StdSemaphore.h StdSemaphore.cpp)
add_library(Threading ${SOURCE_FILES})

