cmake_minimum_required(VERSION 4.0)
project(EX21 C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(EX21
        list.c
        list.h
        listTest.c)
