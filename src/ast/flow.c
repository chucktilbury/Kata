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
void traverse_if_clause(ast_if_clause* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
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
void traverse_else_clause_item(ast_else_clause_item* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
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
void traverse_else_clause(ast_else_clause* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
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
void traverse_switch_clause(ast_switch_clause* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    RET;
}

/**
 * @brief 
 * 
 *  case_item
 *      = literal_value
 *      / LITERAL_DSTR
 *      / LITERAL_SSTR
 * 
 * @param node 
 * @param func 
 * 
 */
void traverse_case_item(ast_case_item* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    RET;
}

/**
 * @brief 
 * 
 *  case_clause
 *      = 'case' '(' case_item ')' function_body
 *
 * @param node 
 * 
 */
void traverse_case_clause(ast_case_clause* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
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
void traverse_default_clause(ast_default_clause* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
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
void traverse_case_body(ast_case_body* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    RET;
}


