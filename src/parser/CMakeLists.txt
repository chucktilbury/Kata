cmake_minimum_required(VERSION 3.10)
project(parser)

set(CMAKE_VERBOSE_MAKEFILE OFF)

add_library(${PROJECT_NAME} STATIC
    ast.c
    parser.c
    module.c
    import.c
    errors.c
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
        $<$<CONFIG:DEBUG>:-g >
        $<$<CONFIG:DEBUG>:-DMEMORY_DEBUG >
        $<$<CONFIG:DEBUG>:-DUSE_TRACE >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)

#add_subdirectory(test)