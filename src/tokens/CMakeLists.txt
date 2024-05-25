cmake_minimum_required(VERSION 3.10)
project(tokens)

include(${PROJECT_SOURCE_DIR}/../../BuildOpts.txt)

add_library(${PROJECT_NAME} STATIC
    tokens.c
)

add_custom_command(
    OUTPUT tokens.c tokens.h
    COMMENT "Make a sorted list of the tokens and associated infrastructure."
    COMMAND python ${PROJECT_SOURCE_DIR}/mk_tokens.py 
                    ${PROJECT_SOURCE_DIR}/keywords.txt 
                    ${PROJECT_SOURCE_DIR}/tokens.h.template 
                    ${PROJECT_SOURCE_DIR}/tokens.c.template
    #SOURCES keywords.txt
    DEPENDS mk_tokens.py keywords.txt tokens.c.template tokens.h.template
)
