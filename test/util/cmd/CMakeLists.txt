cmake_minimum_required(VERSION 3.10)
project(cmd_test)

set(CMAKE_VERBOSE_MAKEFILE OFF)

add_executable(${PROJECT_NAME}
    cmd_test.c
)

target_link_libraries(${PROJECT_NAME}
    pthread
    util
    gc
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -DUSE_GC
        -I${PROJECT_SOURCE_DIR}/..
        -I${PROJECT_SOURCE_DIR}/../../../src/util
        -I${PROJECT_SOURCE_DIR}/../../../src/gc/include
        -I${PROJECT_SOURCE_DIR}/../../../src/parser
        -I${PROJECT_SOURCE_DIR}/../../../src/scanner
        $<$<CONFIG:DEBUG>:-g >
        $<$<CONFIG:DEBUG>:-DENABLE_LOGGER >
        $<$<CONFIG:DEBUG>:-DUSE_TRACE >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)
