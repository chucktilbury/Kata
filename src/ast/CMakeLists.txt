cmake_minimum_required(VERSION 3.10)
project(ast)

include(${PROJECT_SOURCE_DIR}/../../BuildOpts.txt)

add_library(${PROJECT_NAME} STATIC
    ast.c
    strg.c
    vars.c
    compound.c
    except.c
    expr.c
    flow.c
    func_body.c
    func.c
    import.c
    list.c
    literals.c
    loop.c
    module.c
)


