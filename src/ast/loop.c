/**
 * @file loop.c
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
 *  while_definition
 *      = 'while' ( '(' ( expression )? ')' )?
 *      
 * @param node 
 * 
 */
void traverse_while_definition(ast_while_definition* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);
    
    ENTER;
    AST_CALLBACK(pre, node);

    if(node->expr != NULL)
        traverse_expression(node->expr, pre, post);
    else 
        TRACE("no expression");
    
    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief 
 * 
 *  while_clause
 *      = while_definition function_body
 *      
 * @param node 
 * 
 */
void traverse_while_clause(ast_while_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);
    
    ENTER;
    AST_CALLBACK(pre, node);

    traverse_while_definition(node->nterm, pre, post);
    traverse_function_body(node->body, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief 
 * 
 *  do_clause
 *      = 'do' function_body while_definition
 *      
 * @param node 
 * 
 */
void traverse_do_clause(ast_do_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);
    
    ENTER;
    AST_CALLBACK(pre, node);

    traverse_function_body(node->body, pre, post);
    traverse_while_definition(node->nterm, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief 
 * 
 *  for_clause
 *      = 'for' ( '(' (type_name)? SYMBOL 'in' expression ')' )?
 *              function_body
 *      
 * @param node 
 * 
 */
void traverse_for_clause(ast_for_clause* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);
    
    ENTER;
    AST_CALLBACK(pre, node);

    if(node->type != NULL)
        traverse_type_name(node->type, pre, post);
    else 
        TRACE("no type name");
    
    if(node->symbol != NULL)
        TRACE_TERM(node->symbol);
    else 
        TRACE("no symbol");
    
    if(node->expr != NULL)
        traverse_expression(node->expr, pre, post);
    else 
        TRACE("no expression");

    traverse_function_body(node->body, pre, post);

    AST_CALLBACK(post, node);
    RET;
}


