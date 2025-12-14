cmake_minimum_required(VERSION 4.0)
project(GitProject C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(hw2
        correction.h
        correction.c
spellcheck.c
util.c
util.h
)