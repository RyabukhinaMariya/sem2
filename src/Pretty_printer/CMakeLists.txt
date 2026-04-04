cmake_minimum_required(VERSION 3.13)
project(PRETTY PRINTER C)

add_executable(
    pretty_printer.c
)

target_compile_options(pretty_printer PUBLIC -Wall -Wextra -Wpedantic)
