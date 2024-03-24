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
void traverse_try_clause(ast_try_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);
    assert(pre != NULL);
    assert(post != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_function_body(node->fbod, pre, post);
    traverse_except_clause(node->ecla, pre, post);
    
    AST_CALLBACK(post, node);
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
void traverse_except_clause_mid(ast_except_clause_mid* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);
    assert(pre != NULL);
    assert(post != NULL);

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
 *  except_clause_final
 *      = 'except' '(' 'any' ',' SYMBOL ')' function_body
 * 
 * @param node 
 * 
 */
void traverse_except_clause_final(ast_except_clause_final* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);
    assert(pre != NULL);
    assert(post != NULL);
    
    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->msg);
    traverse_function_body(node->fbod, pre, post);

    AST_CALLBACK(post, node);
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
void traverse_except_clause(ast_except_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);
    assert(pre != NULL);
    assert(post != NULL);

    ENTER;
    AST_CALLBACK(pre, node);
    ast_except_clause_mid* mid;

    init_llist_iter(node->list);
    while(NULL != (mid = iter_llist(node->list)))
        traverse_except_clause_mid(mid, pre, post);
    if(node->fin != NULL)
        traverse_except_clause_final(node->fin, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

