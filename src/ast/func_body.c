/**
 * @file func_body.c
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
 * function_body_element
 *      = var_definition
 *      / function_reference
 *      / assignment
 *      / while_clause
 *      / do_clause
 *      / for_clause
 *      / if_clause
 *      / try_clause
 *      / switch_clause
 *      / break_statement
 *      / continue_statement
 *      / trace_statement
 *      / inline_statement
 *      / yield_statement
 *      / type_statement
 *      / exit_statement
 *      / print_statement
 *      / return_statement
 *      / raise_statement
 *      / function_body
 *      
 * @param node 
 * 
 */
void traverse_function_body_element(ast_function_body_element* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  break_statement
 *      = 'break'
 * 
 * @param node 
 * 
 */
void traverse_break_statement(ast_break_statement* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  continue_statement
 *      = 'continue'
 * 
 * @param node 
 * 
 */
void traverse_continue_statement(ast_continue_statement* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  trace_statement
 *      = 'trace'
 * 
 * @param node 
 * 
 */
void traverse_trace_statement(ast_trace_statement* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  inline_statement
 *      = 'inline' '{' RAW_TEXT '}'
 * 
 * @param node 
 * 
 */
void traverse_inline_statement(ast_inline_statement* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  yield_statement
 *      = 'yield' '(' compound_reference ')'
 * 
 * @param node 
 * 
 */
void traverse_yield_statement(ast_yield_statement* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  type_statement
 *      = 'type' '(' compound_reference ')'
 * 
 * @param node 
 * 
 */
void traverse_type_statement(ast_type_statement* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  exit_statement
 *      = 'exit' '(' ( expression )? ')
 * 
 * @param node 
 * 
 */
void traverse_exit_statement(ast_exit_statement* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  print_statement
 *      = 'print' ( expression_list )?
 * 
 * @param node 
 * 
 */
void traverse_print_statement(ast_print_statement* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  return_statement
 *      = 'return' ( '(' ( expression )? ')' )?
 * 
 * @param node 
 * 
 */
void traverse_return_statement(ast_return_statement* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  raise_statement
 *      = 'raise' '(' SYMBOL ',' formatted_strg ')'
 * 
 * @param node 
 * 
 */
void traverse_raise_statement(ast_raise_statement* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}


