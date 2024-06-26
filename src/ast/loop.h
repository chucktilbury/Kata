/**
 * @file loop.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __LOOP_H__
#define __LOOP_H__
#include "ast.h"

/**
 *  while_definition
 *      = 'while' ( '(' ( expression )? ')' )?
 */
typedef struct _ast_while_definition_ {
    ast_node node;
    struct _ast_expression_* expr;
} ast_while_definition;

/**
 *  while_clause
 *      = while_definition function_body
 */
typedef struct _ast_while_clause_ {
    ast_node node;
    struct _ast_while_definition_* nterm;
    struct _ast_function_body_* body;
} ast_while_clause;

/**
 *  do_clause
 *      = 'do' function_body while_definition
 */
typedef struct _ast_do_clause_ {
    ast_node node;
    struct _ast_while_definition_* nterm;
    struct _ast_function_body_* body;
} ast_do_clause;

/**
 *  for_clause
 *      = 'for' ( '(' (type_name)? SYMBOL 'in' expression ')' )?
 *          function_body
 */
typedef struct _ast_for_clause_ {
    ast_node node;
    Token* symbol;
    struct _ast_type_name_* type;
    struct _ast_expression_* expr;
    struct _ast_function_body_* body;
} ast_for_clause;

void traverse_while_definition(ast_while_definition* node, PassFunc pre, PassFunc post);
void traverse_while_clause(ast_while_clause* node, PassFunc pre, PassFunc post);
void traverse_do_clause(ast_do_clause* node, PassFunc pre, PassFunc post);
void traverse_for_clause(ast_for_clause* node, PassFunc pre, PassFunc post);


#endif /* __LOOP_H__ */
