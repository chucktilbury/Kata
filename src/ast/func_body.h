/**
 * @file func_body.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __FUNC_BODY_H__
#define __FUNC_BODY_H__
#include "ast.h"

/**
 *  function_body_element
 *      = var_definition
 *      / function_reference
 *      / create_reference
 *      / destroy_reference
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
 */
typedef struct _ast_function_body_element_ {
    ast_node node;
    ast_node* nterm;
} ast_function_body_element;

/**
 *  break_statement
 *      = 'break'
 */
typedef struct _ast_break_statement_ {
    ast_node node;
} ast_break_statement;

/**
 *  continue_statement
 *      = 'continue'
 */
typedef struct _ast_continue_statement_ {
    ast_node node;
} ast_continue_statement;

/**
 *  trace_statement
 *      = 'trace'
 */
typedef struct _ast_trace_statement_ {
    ast_node node;
} ast_trace_statement;

/**
 *  inline_statement
 *      = 'inline' '{' RAW_TEXT '}'
 */
typedef struct _ast_inline_statement_ {
    ast_node node;
    Token* tok;
} ast_inline_statement;

/**
 *  yield_statement
 *      = 'yield' '(' compound_reference ')'
 */
typedef struct _ast_yield_statement_ {
    ast_node node;
    struct _ast_compound_reference_* ref;
} ast_yield_statement;

/**
 *  type_statement
 *      = 'type' '(' compound_reference ')'
 */
typedef struct _ast_type_statement_ {
    ast_node node;
    struct _ast_compound_reference_* ref;
} ast_type_statement;

/**
 *  exit_statement
 *      = 'exit' '(' ( expression )? ')
 */
typedef struct _ast_exit_statement_ {
    ast_node node;
    struct _ast_expression_* expr;
} ast_exit_statement;

/**
 *  print_statement
 *      = 'print' ( expression_list )?
 */
typedef struct _ast_print_statement_ {
    ast_node node;
    struct _ast_expression_list_* elst;
} ast_print_statement;

/**
 *  return_statement
 *      = 'return' ( '(' ( expression )? ')' )?
 */
typedef struct _ast_return_statement_ {
    ast_node node;
    struct _ast_expression_* expr;
} ast_return_statement;

/**
 *  raise_statement
 *      = 'raise' '(' SYMBOL ',' formatted_strg ')'
 */
typedef struct _ast_raise_statement_ {
    ast_node node;
    Token* symb;
    struct _ast_formatted_strg_* str;
} ast_raise_statement;

void traverse_function_body_element(ast_function_body_element* node, PassFunc pre, PassFunc post);
void traverse_break_statement(ast_break_statement* node, PassFunc pre, PassFunc post);
void traverse_continue_statement(ast_continue_statement* node, PassFunc pre, PassFunc post);
void traverse_trace_statement(ast_trace_statement* node, PassFunc pre, PassFunc post);
void traverse_inline_statement(ast_inline_statement* node, PassFunc pre, PassFunc post);
void traverse_yield_statement(ast_yield_statement* node, PassFunc pre, PassFunc post);
void traverse_type_statement(ast_type_statement* node, PassFunc pre, PassFunc post);
void traverse_exit_statement(ast_exit_statement* node, PassFunc pre, PassFunc post);
void traverse_print_statement(ast_print_statement* node, PassFunc pre, PassFunc post);
void traverse_return_statement(ast_return_statement* node, PassFunc pre, PassFunc post);
void traverse_raise_statement(ast_raise_statement* node, PassFunc pre, PassFunc post);


#endif /* __FUNC_BODY_H__ */
