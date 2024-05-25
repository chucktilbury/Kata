project(common)

include(${PROJECT_SOURCE_DIR}/../../BuildOpts.txt)

add_library(${PROJECT_NAME} STATIC
    errors.c
    memory.c
    link_list.c
    trace.c
    sstrings.c
    paths.c
    exception.c
)



