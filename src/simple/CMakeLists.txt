cmake_minimum_required(VERSION 3.10)
project(simple)

set(CMAKE_VERBOSE_MAKEFILE OFF)

add_executable(${PROJECT_NAME}
    command_line.c
    simple.c
)

target_link_libraries(${PROJECT_NAME}
    pthread
    misc
    parse
    scan
    ast
    pass
    gc
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -DUSE_GC
        -I${PROJECT_SOURCE_DIR}/..
        -I${PROJECT_SOURCE_DIR}/../util
        -I${PROJECT_SOURCE_DIR}/../util/gc/include
        -I${PROJECT_SOURCE_DIR}/../parse
        -I${PROJECT_SOURCE_DIR}/../scanner
        -I${PROJECT_SOURCE_DIR}/../ast
        -I${PROJECT_SOURCE_DIR}/../pass
        -I${PROJECT_SOURCE_DIR}/../misc
        -I${CMAKE_CURRENT_BINARY_DIR}/../scanner
        $<$<CONFIG:DEBUG>:-g >
        $<$<CONFIG:DEBUG>:-DENABLE_LOGGER >
        $<$<CONFIG:DEBUG>:-DUSE_TRACE >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)
