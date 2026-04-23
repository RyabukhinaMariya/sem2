cmake_minimum_required(VERSION 3.13)
project(pretty_printer C)

add_executable(program
    main.c
    airports_io.c
    avlTree.c
)

add_executable(tests
    tests.c
    airports_io.c
    avlTree.c
)

target_compile_options(program PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)
