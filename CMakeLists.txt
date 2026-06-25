cmake_minimum_required(VERSION 3.20)
project(PROGRAMMING_LANGUAGE C)

set(CMAKE_C_STANDARD 11)

add_executable(main
        test.c
        lexer.c
)