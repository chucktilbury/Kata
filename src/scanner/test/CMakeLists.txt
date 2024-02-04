cmake_minimum_required(VERSION 3.10)
project(scanner_test)

set(CMAKE_VERBOSE_MAKEFILE OFF)

add_executable(${PROJECT_NAME}
    scanner_test.c
)

target_link_libraries(${PROJECT_NAME}
    pthread
    scanner
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
        -I${PROJECT_SOURCE_DIR}/../../util
        -I${PROJECT_SOURCE_DIR}/../../scanner
        -I${PROJECT_SOURCE_DIR}/../../parser
        -I${PROJECT_SOURCE_DIR}/../../bdwgc/include
        $<$<CONFIG:DEBUG>:-g >
        $<$<CONFIG:DEBUG>:-DENABLE_LOGGER >
        $<$<CONFIG:DEBUG>:-DENABLE_DUMP >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)

add_executable(queue_test
    queue_test.c
)

target_link_libraries(queue_test
    pthread
    scanner
    parser
    util
    gc
)

target_compile_options(queue_test
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -DUSE_GC
        -I${PROJECT_SOURCE_DIR}/..
        -I${PROJECT_SOURCE_DIR}/../../util
        -I${PROJECT_SOURCE_DIR}/../../scanner
        -I${PROJECT_SOURCE_DIR}/../../parser
        -I${PROJECT_SOURCE_DIR}/../../bdwgc/include
        $<$<CONFIG:DEBUG>:-g >
        $<$<CONFIG:DEBUG>:-DENABLE_LOGGER >
        $<$<CONFIG:DEBUG>:-DENABLE_DUMP >
        $<$<CONFIG:RELEASE>:-Ofast >
        $<$<CONFIG:PROFILE>:-pg -O0 >
)

