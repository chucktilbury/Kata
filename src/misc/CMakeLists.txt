cmake_minimum_required(VERSION 3.10)
project(misc)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME} STATIC
    errors.c
    memory.c
    link_list.c
    trace.c
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -DUSE_GC
        -I${PROJECT_SOURCE_DIR}/../util
        -I${PROJECT_SOURCE_DIR}/../scanner
        -I${PROJECT_SOURCE_DIR}/../parse
        -I${PROJECT_SOURCE_DIR}/../ast
        -I${PROJECT_SOURCE_DIR}/../pass
        -I${PROJECT_SOURCE_DIR}
        -I${CMAKE_CURRENT_BINARY_DIR}/../scanner
        $<$<CONFIG:DEBUG>:-g >
        $<$<CONFIG:DEBUG>:-DMEMORY_DEBUG >
        $<$<CONFIG:DEBUG>:-DUSE_TRACE >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)

