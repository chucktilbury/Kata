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
    
    AST_CALLBACK(func, node);
    traverse_expression(node->expr, func);
    traverse_function_body(node->fbod, func);
    traverse_else_clause(node->eclaus, func);

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
    AST_CALLBACK(func, node);
    TRACE("type: %s", nterm_to_str((ast_node*)node));
    traverse_expression(node->expr, func);
    traverse_function_body(node->fbod, func);
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
    AST_CALLBACK(func, node);
    init_llist_iter(node->list);
    ast_else_clause_item* item;
    while(NULL != (item = iter_llist(node->list)))
        traverse_else_clause_item(item, func);
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
    AST_CALLBACK(func, node);
    traverse_compound_reference(node->cref, func);
    traverse_case_body(node->cbod, func);
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
    AST_CALLBACK(func, node);
    if(node->lstr != NULL)
        TRACE_TERM(node->lstr);
    else if(node->lval != NULL)
        traverse_literal_value(node->lval, func);
    else
        RAISE(TRAVERSE_ERROR, "no object for a case item");
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
    AST_CALLBACK(func, node);
    traverse_case_item(node->item, func);
    traverse_function_body(node->fbod, func);
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
    AST_CALLBACK(func, node);
    traverse_function_body(node->fbod, func);
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
    AST_CALLBACK(func, node);
    init_llist_iter(node->list);
    ast_node* nterm;
    while(NULL != (nterm = node->list)) {
        switch(ast_node_type(nterm)) {
            case AST_case_clause:
                traverse_case_clause((ast_case_clause*)nterm, func);
                break;
            case AST_default_clause:
                traverse_default_clause((ast_default_clause*)nterm, func);
                break;
            default:
                RAISE(TRAVERSE_ERROR, "unknown node type in %s: %s", __func__, nterm_to_str(nterm));
        }
    }
    RET;
}


