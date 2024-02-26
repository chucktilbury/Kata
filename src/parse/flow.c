/**
 * @file flow.c
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

/**
 * @brief 
 * 
 *  if_clause
 *      = 'if' '(' expression ')' function_body else_clause
 *      
 * @return ast_if_clause* 
 * 
 */
ast_if_clause* parse_if_clause() {

    ENTER;
    ast_if_clause* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  else_clause_mid
 *      = 'else' '(' expression ')' function_body
 *      
 * @return ast_else_clause_mid* 
 * 
 */
ast_else_clause_mid* parse_else_clause_mid() {

    ENTER;
    ast_else_clause_mid* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  else_clause_final
 *      = 'else' ( '(' ')' )? function_body
 *      
 * @return ast_else_clause_final* 
 * 
 */
ast_else_clause_final* parse_else_clause_final() {

    ENTER;
    ast_else_clause_final* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  else_clause
 *      = ( else_clause_mid )* ( else_clause_final )?
 *      
 * @return ast_else_clause* 
 * 
 */
ast_else_clause* parse_else_clause() {

    ENTER;
    ast_else_clause* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  switch_clause
 *      = 'switch' '(' compound_reference ')' case_body
 *      
 * @return ast_switch_clause* 
 * 
 */
ast_switch_clause* parse_switch_clause() {

    ENTER;
    ast_switch_clause* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  case_clause
 *      = 'case' '(' ( literal_value / LITERAL_DSTR / LITERAL_SSTR ) ')' function_body
 *      
 * @return ast_case_clause* 
 * 
 */
ast_case_clause* parse_case_clause() {

    ENTER;
    ast_case_clause* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  default_clause
 *      = 'default' function_body
 *      
 * @return ast_default_clause* 
 * 
 */
ast_default_clause* parse_default_clause() {

    ENTER;
    ast_default_clause* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  case_body
 *      = '{' ( case_clause_list )+ ( default_clause )? '}'
 *      
 * @return ast_case_body* 
 * 
 */
ast_case_body* parse_case_body() {

    ENTER;
    ast_case_body* node = NULL;

    RETV(node);
}


