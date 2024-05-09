/**
 * @file flow.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#include <assert.h>

#include "ast.h"
#include "errors.h"
#include "trace.h"

/**
 * @brief
 *
 *  if_clause
 *      = 'if' '(' expression ')' function_body ( elif_clause )* ( else_clause )?
 *
 * @param node
 *
 */
void traverse_if_clause(ast_if_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    void* mark = NULL;
    ast_node* nterm;

    traverse_expression(node->expr, pre, post);
    traverse_function_body(node->fbod, pre, post);

    // normally this list will simply be empty...
    if(NULL != node->elif_list) {
        while(NULL != (nterm = iter_link_list(node->elif_list, &mark)))
            traverse_elif_clause((ast_elif_clause*)nterm, pre, post);
    }

    if(NULL != node->eclaus)
        traverse_else_clause(node->eclaus, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  else_clause
 *      = 'else' function_body
 *
 * @param node
 *
 */
void traverse_else_clause(ast_else_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_function_body(node->fbod, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  elif_clause
 *      = 'elif' '(' expression ')' function_body
 *
 * @param node
 *
 */
void traverse_elif_clause(ast_elif_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_expression(node->expr, pre, post);
    traverse_function_body(node->fbod, pre, post);

    AST_CALLBACK(post, node);
    RET;
}


