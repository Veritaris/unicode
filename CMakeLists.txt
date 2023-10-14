cmake_minimum_required(VERSION 3.26)
project(unicode C)

set(CMAKE_C_STANDARD 11)

add_library(
        unicode
        src/unicode.c
        src/unicode_benchmark.c
        src/main.c
)
