cmake_minimum_required(VERSION 3.24)
project(text_justification_v2 C)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(text_justification_v2
        main.c headers.h just_functions.c)
