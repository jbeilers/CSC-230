cmake_minimum_required(VERSION 4.0)
project(EX07 C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(EX07
        main.c
        names.c
        names.h
        scores.c
        scores.h
        report.c
        report.h)
