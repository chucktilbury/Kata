cmake_minimum_required(VERSION 3.10)
project(kata_compiler)

set(CMAKE_VERBOSE_MAKEFILE OFF )
#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
#set(CMAKE_C_COMPILER /usr/bin/gcc)
#set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

add_subdirectory(src)
add_subdirectory(test EXCLUDE_FROM_ALL)
add_subdirectory(docs EXCLUDE_FROM_ALL)

file(GLOB_RECURSE format_xxfiles
    ${PROJECT_SOURCE_DIR}/src/parser/*.c
    ${PROJECT_SOURCE_DIR}/src/parser/*.h
    ${PROJECT_SOURCE_DIR}/src/scanner/*.c
    ${PROJECT_SOURCE_DIR}/src/scanner/*.h
    ${PROJECT_SOURCE_DIR}/src/kata/*.c
    ${PROJECT_SOURCE_DIR}/src/kata/*.h
    ${PROJECT_SOURCE_DIR}/src/ast/*.c
    ${PROJECT_SOURCE_DIR}/src/ast/*.h
    ${PROJECT_SOURCE_DIR}/src/symbols/*.c
    ${PROJECT_SOURCE_DIR}/src/symbols/*.h
    ${PROJECT_SOURCE_DIR}/src/common/*.c
    ${PROJECT_SOURCE_DIR}/src/common/*.h
    ${PROJECT_SOURCE_DIR}/test/*.c
    ${PROJECT_SOURCE_DIR}/test/*.h
)

add_custom_target(format
    COMMENT "Format the source code"
    COMMAND clang-format --verbose -i ${format_xxfiles}
)

add_custom_target(testing
    COMMENT "Build the test programs"
    COMMAND echo ${PROJECT_SOURCE_DIR}/test
    COMMAND make -C test
)

