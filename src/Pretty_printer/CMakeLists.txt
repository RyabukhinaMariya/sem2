cmake_minimum_required(VERSION 3.13)
project(pretty_printer C)

add_executable(pretty_printer
    main.c
    createTable.c
)

target_compile_options(pretty_printer PUBLIC -Wall -Wextra -Wpedantic)
