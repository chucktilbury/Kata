/**
 * @file flow.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __FLOW_H__
#define __FLOW_H__
#include "ast.h"

/**
 *  if_clause
 *      = 'if' '(' expression ')' function_body ( elif_clause )* ( else_clause )?
 */
typedef struct _ast_if_clause_ {
    ast_node node;
    struct _ast_expression_* expr;
    struct _ast_function_body_* fbod;
    struct _ast_else_clause_* eclaus;
    LinkList* elif_list;
} ast_if_clause;

/**
 *  elif_clause
 *      = 'elif' '(' expression ')' function_body
 */
typedef struct _ast_elif_clause_ {
    ast_node node;
    struct _ast_expression_* expr; // expression is empty for final
    struct _ast_function_body_* fbod;
} ast_elif_clause;

/**
 *  else_clause
 *      = 'else' function_body
 */
typedef struct _ast_else_clause_ {
    ast_node node;
    struct _ast_function_body_* fbod;
} ast_else_clause;

void traverse_if_clause(ast_if_clause* node, PassFunc pre, PassFunc post);
void traverse_elif_clause(ast_elif_clause* node, PassFunc pre, PassFunc post);
void traverse_else_clause(ast_else_clause* node, PassFunc pre, PassFunc post);


#endif /* __FLOW_H__ */
