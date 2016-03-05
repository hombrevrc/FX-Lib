cmake_minimum_required(VERSION 3.3)
project(Console)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES stdafx.cpp Console.cpp)
add_executable(Console ${SOURCE_FILES})


target_link_libraries(Console System Threading Logging stdc++ stdc++fs pthread)