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
} ast_if_clause;

/**
 *  else_clause_mid
 *      = 'else' '(' expression ')' function_body
 */
typedef struct _ast_else_clause_mid_ {
    ast_node node;
} ast_else_clause_mid;

/**
 *  else_clause_final
 *      = 'else' ( '(' ')' )? function_body
 */
typedef struct _ast_else_clause_final_ {
    ast_node node;
} ast_else_clause_final;

/**
 *  else_clause
 *      = ( else_clause_mid )* ( else_clause_final )?
 */
typedef struct _ast_else_clause_ {
    ast_node node;
} ast_else_clause;

/**
 *  switch_clause
 *      = 'switch' '(' compound_reference ')' case_body
 */
typedef struct _ast_switch_clause_ {
    ast_node node;
} ast_switch_clause;

/**
 *  case_clause
 *      = 'case' '(' ( literal_value / LITERAL_DSTR / LITERAL_SSTR ) ')' 
 *          function_body
 */
typedef struct _ast_case_clause_ {
    ast_node node;
} ast_case_clause;

/**
 *  default_clause
 *      = 'default' function_body
 */
typedef struct _ast_default_clause_ {
    ast_node node;
} ast_default_clause;

/**
 *  case_body
 *      = '{' ( case_clause_list )+ ( default_clause )? '}'
 */
typedef struct _ast_case_body_ {
    ast_node node;
} ast_case_body;

void traverse_if_clause(ast_if_clause* node);
void traverse_else_clause_mid(ast_else_clause_mid* node);
void traverse_else_clause_final(ast_else_clause_final* node);
void traverse_else_clause(ast_else_clause* node);
void traverse_switch_clause(ast_switch_clause* node);
void traverse_case_clause(ast_case_clause* node);
void traverse_default_clause(ast_default_clause* node);
void traverse_case_body(ast_case_body* node);


#endif /* __FLOW_H__ */
