cmake_minimum_required(VERSION 3.13)
project(kingdoms C)
add_executable(pretty_printer
    states.c
    states.h
)

target_compile_options(pretty_printer PUBLIC -Wall -Wextra -Wpedantic)
