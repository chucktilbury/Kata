cmake_minimum_required(VERSION 3.10)
project(scan)

#set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME} STATIC
    scanner.c
    tok_queue.c
    fileio.c
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -DUSE_GC
        -I${PROJECT_SOURCE_DIR}
        -I${PROJECT_SOURCE_DIR}/../util
        -I${PROJECT_SOURCE_DIR}/../common
        -I${PROJECT_SOURCE_DIR}/../ast
        -I${PROJECT_SOURCE_DIR}/../scanner
        -I${PROJECT_SOURCE_DIR}/../pass
        -I${PROJECT_SOURCE_DIR}/../parse
        -I${PROJECT_SOURCE_DIR}/../gc/include
        -I${CMAKE_CURRENT_BINARY_DIR}/../tokens
        -I${CMAKE_CURRENT_BINARY_DIR}
        $<$<CONFIG:DEBUG>:-g >
        $<$<CONFIG:DEBUG>:-DMEMORY_DEBUG >
        $<$<CONFIG:DEBUG>:-DUSE_TRACE >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)

#include_directories(${CMAKE_CURRENT_BINARY_DIR} ./)

# add_custom_command(
#     OUTPUT tokens.c tokens.h
#     COMMENT "Make a sorted list of the tokens and associated infrastructure."
#     COMMAND python ${PROJECT_SOURCE_DIR}/mk_tokens.py ${PROJECT_SOURCE_DIR}/keywords.txt
#     #SOURCES keywords.txt
#     DEPENDS mk_tokens.py keywords.txt
# )