cmake_minimum_required(VERSION 3.13)
project(BST C)

add_executable(bst
    pretty_printer.c
)

target_compile_options(bst PUBLIC -Wall -Wextra -Wpedantic)