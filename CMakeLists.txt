cmake_minimum_required(VERSION 3.15)
project(untitled1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(untitled1 main.cpp Queue.cpp Queue.h)