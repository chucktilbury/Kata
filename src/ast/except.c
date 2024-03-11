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
#define USE_TRACE 1
#include "util.h"
#include "ast.h"

/**
 * @brief 
 * 
 *  try_clause
 *      = 'try' function_body except_clause
 * 
 * @param node 
 * 
 */
void traverse_try_clause(ast_try_clause* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);

    traverse_function_body(node->fbod, func);
    traverse_except_clause(node->ecla, func);
    
    RET;
}

/**
 * @brief 
 * 
 *  except_clause_mid
 *      = 'except' '(' SYMBOL ',' SYMBOL ')' function_body
 * 
 * @param node 
 * 
 */
void traverse_except_clause_mid(ast_except_clause_mid* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);

    TRACE_TERM(node->name);
    TRACE_TERM(node->msg);
    traverse_function_body(node->fbod, func);

    RET;
}

/**
 * @brief 
 * 
 *  except_clause_final
 *      = 'except' '(' 'any' ',' SYMBOL ')' function_body
 * 
 * @param node 
 * 
 */
void traverse_except_clause_final(ast_except_clause_final* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    AST_CALLBACK(func, node);

    TRACE_TERM(node->msg);
    traverse_function_body(node->fbod, func);

    RET;
}

/**
 * @brief 
 * 
 *  except_clause
 *      = ( except_clause_mid )+ ( except_clause_final )?
 *      / except_clause_final
 * 
 * @param node 
 * 
 */
void traverse_except_clause(ast_except_clause* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    ast_except_clause_mid* mid;

    init_llist_iter(node->list);
    while(NULL != (mid = iter_llist(node->list)))
        traverse_except_clause_mid(mid, func);
    if(node->fin != NULL)
        traverse_except_clause_final(node->fin, func);
    RET;
}

