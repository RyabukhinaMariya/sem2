cmake_minimum_required(VERSION 3.13)
project(KINDOMS C)

add_executable(
    kindoms.c
)

target_compile_options(kindoms PUBLIC -Wall -Wextra -Wpedantic)
