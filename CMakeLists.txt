cmake_minimum_required(VERSION 3.5)
project(rdpgen_exe)

#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_BUILD_TYPE release)
#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
#set(CMAKE_C_COMPILER /usr/bin/gcc)
#set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(CMAKE_BUILD_TYPE debug)

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
#set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${LIBRARY_OUTPUT_PATH}")
#set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${EXECUTABLE_OUTPUT_PATH}")

add_subdirectory(src)
add_subdirectory(docs EXCLUDE_FROM_ALL)

file(GLOB_RECURSE format_xxfiles ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*.h)
add_custom_target(format
    COMMENT "Format the source code"
    COMMAND clang-format --verbose -i ${format_xxfiles}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src
)


