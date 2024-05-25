/**
 * @file except.c
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
#include "link_list.h"
#include "trace.h"

/**
 * @brief
 *
 *  try_clause
 *      = 'try' function_body ( except_clause )* ( final_clause )?
 *
 * @param node
 *
 */
void traverse_try_clause(ast_try_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_function_body(node->fbod, pre, post);

    void* mark = NULL;
    ast_except_clause* ecl;

    if(NULL != node->except_list) {
        while(NULL != (ecl = iter_link_list(node->except_list, &mark)))
            traverse_except_clause(ecl, pre, post);
    }

    if(NULL != node->final)
        traverse_final_clause(node->final, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  except_clause
 *      = 'except' '(' SYMBOL ',' SYMBOL ')' function_body
 *
 * @param node
 *
 */
void traverse_except_clause(ast_except_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->name);
    TRACE_TERM(node->msg);
    traverse_function_body(node->fbod, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  final_clause
 *      = 'final' '(' SYMBOL ')' function_body
 *
 * @param node
 *
 */
void traverse_final_clause(ast_final_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->msg);
    traverse_function_body(node->fbod, pre, post);

    AST_CALLBACK(post, node);
    RET;
}
