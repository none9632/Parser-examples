cmake_minimum_required(VERSION 3.15)
project(LL C)

set(CMAKE_C_STANDARD 99)

add_executable(LL
        src/main.c
        src/lexer.c
        src/lexer.h
        src/parser.c
        src/parser.h)
