cmake_minimum_required(VERSION 3.13)
project(AIRPORTS C)

add_executable(
    airports.c
)

target_compile_options(airports PUBLIC -Wall -Wextra -Wpedantic)
