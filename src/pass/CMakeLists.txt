cmake_minimum_required(VERSION 3.10)
project(pass)

include(${PROJECT_SOURCE_DIR}/../../BuildOpts.txt)

add_library(${PROJECT_NAME} STATIC
    testpass.c
    sympass.c
)


