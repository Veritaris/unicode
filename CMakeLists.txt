cmake_minimum_required(VERSION 3.26)

project(unicode
    VERSION 1.0.0
    DESCRIPTION "Unicode support for C in pure C"
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# ============ Lib implementation ============ #
add_library(unicode "${CMAKE_CURRENT_SOURCE_DIR}/src/unicode.c")

target_include_directories(unicode
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/private"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/public"
)

# =========== Tests implementation ============= #
add_executable(unicode-test
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/main.c"
)
target_include_directories(unicode-test
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/examples/include"
)
target_link_libraries(unicode-test
    PRIVATE unicode
)