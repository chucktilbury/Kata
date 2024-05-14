cmake_minimum_required(VERSION 3.10)
project(parse)

#set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME} STATIC
    parse.c
    parse_state.c
    strg.c
    vars.c
    compound.c
    except.c
    expr.c
    flow.c
    func_body.c
    func.c
    import.c
    list.c
    literals.c
    loop.c
    module.c
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
        -I${PROJECT_SOURCE_DIR}/../common
        -I${PROJECT_SOURCE_DIR}/../symbols
        -I${CMAKE_CURRENT_BINARY_DIR}/../tokens
        -I${CMAKE_CURRENT_BINARY_DIR}
        $<$<CONFIG:DEBUG>:-g >
        $<$<CONFIG:DEBUG>:-DMEMORY_DEBUG >
        $<$<CONFIG:DEBUG>:-DUSE_TRACE >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)

