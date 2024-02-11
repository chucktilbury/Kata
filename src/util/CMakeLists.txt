cmake_minimum_required(VERSION 3.10)
project(util)

set(CMAKE_VERBOSE_MAKEFILE OFF)
#add_subdirectory(bdwgc)
add_library(${PROJECT_NAME} STATIC
    fileio.c
    hash.c
    str.c
    mem.c
    cmd.c
    except.c
    list.c
    trace.c
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -DUSE_GC
        -I${PROJECT_SOURCE_DIR}../gc/include
        $<$<CONFIG:DEBUG>:-g >
        $<$<CONFIG:DEBUG>:-DMEMORY_DEBUG >
        $<$<CONFIG:DEBUG>:-DUSE_TRACE >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)

