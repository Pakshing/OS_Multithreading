cmake_minimum_required(VERSION 3.17)
project(UniqueWords C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_library(UniqueWords library.c library.h)