cmake_minimum_required(VERSION 3.3)
project(System)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES stdafx.cpp Timeout.cpp FileSystem.cpp Functions.cpp MemoryStream.cpp SystemClock.h
                 SystemClock.cpp SystemTimePoint.h SystemTimePoint.cpp
                 DateTime.h DateTime.cpp SystemException.h SystemException.cpp
                 System.h)

add_library(System ${SOURCE_FILES})

