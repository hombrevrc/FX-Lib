cmake_minimum_required(VERSION 3.3)
project(Logging)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES BinLogger.h BinLogger.cpp BinFlushingQueue.h BinFlushingQueue.cpp BinEntry.h BinEntry.cpp
                 BinStream.h BinStream.cpp stdafx.h stdafx.cpp Logging.h)

add_library(Logging ${SOURCE_FILES})
target_link_libraries(Logging System Threading )

