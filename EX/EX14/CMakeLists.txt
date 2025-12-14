cmake_minimum_required(VERSION 4.0)
project(EX14 C)

set(CMAKE_C_STANDARD 99)

add_executable(EX14
        gps.c)

link_libraries(gps.c m)
