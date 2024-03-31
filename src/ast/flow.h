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
 *      = 'if' '(' expression ')' function_body else_clause
 */
typedef struct _ast_if_clause_ {
    ast_node node;
    struct _ast_expression_* expr;
    struct _ast_function_body_* fbod;
    struct _ast_else_clause_* eclaus;
} ast_if_clause;

/**
 *  else_clause_mid
 *      = 'else' '(' expression ')' function_body
 *  else_clause_final
 *      = 'else' ( '(' ')' )? function_body
 */
typedef struct _ast_else_clause_item_ {
    ast_node node;
    struct _ast_expression_* expr; // expression is empty for final
    struct _ast_function_body_* fbod;
} ast_else_clause_item;

/**
This is no longer valid. Left here for a reminder
typedef struct _ast_else_clause_final_ {
    ast_node node;
    struct _ast_function_body_* fbod;
} ast_else_clause_final;
 */

/**
 *  else_clause
 *      = ( else_clause_mid )* ( else_clause_final )?
 */
typedef struct _ast_else_clause_ {
    ast_node node;
    LinkList* list; // expects to have a final as the end or only.
} ast_else_clause;

/**
 *  switch_clause
 *      = 'switch' '(' compound_reference ')' case_body
 */
typedef struct _ast_switch_clause_ {
    ast_node node;
    struct _ast_compound_reference_* cref;
    struct _ast_case_body_* cbod;
} ast_switch_clause;

/**
 *  case_item
 *      = literal_value
 *      / LITERAL_DSTR
 *      / LITERAL_SSTR
 */
typedef struct _ast_case_item_ {
    ast_node node;
    Token* lstr;
    struct _ast_literal_value_* lval;
} ast_case_item;

/**
 *  case_clause
 *      = 'case' '(' case_item ')' function_body
 */
typedef struct _ast_case_clause_ {
    ast_node node;
    struct _ast_case_item_* item;
    struct _ast_function_body_* fbod;
} ast_case_clause;

/**
 *  default_clause
 *      = 'default' function_body
 */
typedef struct _ast_default_clause_ {
    ast_node node;
    struct _ast_function_body_* fbod;
} ast_default_clause;

/**
 *  case_body
 *      = '{' ( case_clause_list )+ ( default_clause )? '}'
 */
typedef struct _ast_case_body_ {
    ast_node node;
    LinkList* list;
} ast_case_body;

void traverse_if_clause(ast_if_clause* node, PassFunc pre, PassFunc post);
void traverse_else_clause_item(ast_else_clause_item* node, PassFunc pre, PassFunc post);
void traverse_else_clause(ast_else_clause* node, PassFunc pre, PassFunc post);
void traverse_switch_clause(ast_switch_clause* node, PassFunc pre, PassFunc post);
void traverse_case_clause(ast_case_clause* node, PassFunc pre, PassFunc post);
void traverse_default_clause(ast_default_clause* node, PassFunc pre, PassFunc post);
void traverse_case_body(ast_case_body* node, PassFunc pre, PassFunc post);


#endif /* __FLOW_H__ */
