cmake_minimum_required(VERSION 3.5)
project(CPP_SIgnals)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp signals.h)
add_executable(CPP_SIgnals ${SOURCE_FILES})