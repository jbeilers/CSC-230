cmake_minimum_required(VERSION 4.0)
project(EX05 C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(EX05
        game.c
        game.h
        input.c
        input.h
        summary.c
        summary.h
        tournament.c)
