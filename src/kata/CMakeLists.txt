project(kata)

include(${PROJECT_SOURCE_DIR}/../../BuildOpts.txt)

add_executable(${PROJECT_NAME}
    command_line.c
    kata.c
)

target_link_libraries(${PROJECT_NAME}
    parse
    tokens
    scan
    ast
    pass
    symbols
    common
)


