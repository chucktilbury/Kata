
cmake_minimum_required(VERSION 3.10)
project(symbols)

include(${PROJECT_SOURCE_DIR}/../../BuildOpts.txt)

add_library(${PROJECT_NAME} STATIC
    symbols.c
    hash.c
    context.c
)


