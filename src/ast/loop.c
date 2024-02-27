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
void traverse_while_definition(ast_while_definition* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
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
void traverse_while_clause(ast_while_clause* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
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
void traverse_do_clause(ast_do_clause* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
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
void traverse_for_clause(ast_for_clause* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
    RET;
}


