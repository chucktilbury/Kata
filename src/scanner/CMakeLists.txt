cmake_minimum_required(VERSION 3.10)
project(scan)

#set(CMAKE_VERBOSE_MAKEFILE ON)
include(${PROJECT_SOURCE_DIR}/../../BuildOpts.txt)

add_library(${PROJECT_NAME} STATIC
    scanner.c
    tok_queue.c
    fileio.c
)


#include_directories(${CMAKE_CURRENT_BINARY_DIR} ./)

# add_custom_command(
#     OUTPUT tokens.c tokens.h
#     COMMENT "Make a sorted list of the tokens and associated infrastructure."
#     COMMAND python ${PROJECT_SOURCE_DIR}/mk_tokens.py ${PROJECT_SOURCE_DIR}/keywords.txt
#     #SOURCES keywords.txt
#     DEPENDS mk_tokens.py keywords.txt
# )
