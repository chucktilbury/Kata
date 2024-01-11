cmake_minimum_required(VERSION 3.10)
project(libsapcc)

set(CMAKE_VERBOSE_MAKEFILE OFF)

add_library(${PROJECT_NAME} STATIC
    library.c
    tok_queue.c
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -DUSE_GC
        -I${PROJECT_SOURCE_DIR}/../util
        -I${PROJECT_SOURCE_DIR}/../bdwgc/include
        $<$<CONFIG:DEBUG>:-g >
        $<$<CONFIG:DEBUG>:-DMEMORY_DEBUG >
        $<$<CONFIG:DEBUG>:-DENA_TRACE >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)
