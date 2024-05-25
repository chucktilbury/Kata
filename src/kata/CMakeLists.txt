cmake_minimum_required(VERSION 3.10)
project(kata)

include(${PROJECT_SOURCE_DIR}/../../BuildOpts.txt)

add_executable(${PROJECT_NAME}
    command_line.c
    kata.c
)

target_link_libraries(${PROJECT_NAME}
    pthread
    parse
    tokens
    scan
    ast
    pass
    symbols
    common
    gc
)


