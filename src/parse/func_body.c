/**
 * @file func_body.c
 * 
 * @brief 
 * 
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-26-2024
 * @copyright Copyright (c) 2024
 */
#define USE_TRACE 1
#include "util.h"
#include "parse.h"
#include "scanner.h"

/**
 * @brief 
 * 
 *  function_body_element
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
 * @return ast_function_body_element* 
 * 
 */
ast_function_body_element* parse_function_body_element() {

    ENTER;
    ast_function_body_element* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  break_statement
 *      = 'break'
 *      
 * @return ast_break_statement* 
 * 
 */
ast_break_statement* parse_break_statement() {

    ENTER;
    ast_break_statement* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  continue_statement
 *      = 'continue'
 *      
 * @return ast_continue_statement* 
 * 
 */
ast_continue_statement* parse_continue_statement() {

    ENTER;
    ast_continue_statement* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  trace_statement
 *      = 'trace'
 *      
 * @return ast_trace_statement* 
 * 
 */
ast_trace_statement* parse_trace_statement() {

    ENTER;
    ast_trace_statement* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  inline_statement
 *      = 'inline' '{' RAW_TEXT '}'
 *      
 * @return ast_inline_statement* 
 * 
 */
ast_inline_statement* parse_inline_statement() {

    ENTER;
    ast_inline_statement* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  yield_statement
 *      = 'yield' '(' compound_reference ')'
 *      
 * @return ast_yield_statement* 
 * 
 */
ast_yield_statement* parse_yield_statement() {

    ENTER;
    ast_yield_statement* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  type_statement
 *      = 'type' '(' compound_reference ')'
 *      
 * @return ast_type_statement* 
 * 
 */
ast_type_statement* parse_type_statement() {

    ENTER;
    ast_type_statement* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  exit_statement
 *      = 'exit' '(' ( expression )? ')
 *      
 * @return ast_exit_statement* 
 * 
 */
ast_exit_statement* parse_exit_statement() {

    ENTER;
    ast_exit_statement* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  print_statement
 *      = 'print' ( expression_list )?
 *      
 * @return ast_print_statement* 
 * 
 */
ast_print_statement* parse_print_statement() {

    ENTER;
    ast_print_statement* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  return_statement
 *      = 'return' ( '(' ( expression )? ')' )?
 *      
 * @return ast_return_statement* 
 * 
 */
ast_return_statement* parse_return_statement() {

    ENTER;
    ast_return_statement* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  raise_statement
 *      = 'raise' '(' SYMBOL ',' formatted_strg ')'
 *      
 * @return ast_raise_statement* 
 * 
 */
ast_raise_statement* parse_raise_statement() {

    ENTER;
    ast_raise_statement* node = NULL;

    RETV(node);
}


