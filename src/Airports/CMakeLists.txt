cmake_minimum_required(VERSION 3.13)
project(airports C)

add_executable(program
    main.c
    commands.c
    avlTree.c
)

add_executable(tests
    tests.c
    commands.c
    avlTree.c
)

target_compile_options(program PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)
