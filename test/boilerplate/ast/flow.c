/**
 * @file flow.c
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
 *  if_clause
 *      = 'if' '(' expression ')' function_body else_clause
 * 
 * @param node 
 * 
 */
void traverse_if_clause(ast_if_clause* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  else_clause_mid
 *      = 'else' '(' expression ')' function_body
 * 
 * @param node 
 * 
 */
void traverse_else_clause_mid(ast_else_clause_mid* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  else_clause_final
 *      = 'else' ( '(' ')' )? function_body
 * 
 * @param node 
 * 
 */
void traverse_else_clause_final(ast_else_clause_final* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  else_clause
 *      = ( else_clause_mid )* ( else_clause_final )?
 * 
 * @param node 
 * 
 */
void traverse_else_clause(ast_else_clause* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  switch_clause
 *      = 'switch' '(' compound_reference ')' case_body
 * 
 * @param node 
 * 
 */
void traverse_switch_clause(ast_switch_clause* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  case_clause
 *      = 'case' '(' ( literal_value / LITERAL_DSTR / LITERAL_SSTR ) ')' 
 *          function_body
 *
 * @param node 
 * 
 */
void traverse_case_clause(ast_case_clause* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  default_clause
 *      = 'default' function_body
 * 
 * @param node 
 * 
 */
void traverse_default_clause(ast_default_clause* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  case_body
 *      = '{' ( case_clause_list )+ ( default_clause )? '}'
 * 
 * @param node 
 * 
 */
void traverse_case_body(ast_case_body* node) {

    ENTER;
    RET;



