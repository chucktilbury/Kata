/**
 * @file vars.c
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
 *  var_decl
 *      = ( 'const' )? type_name SYMBOL
 *
 * @param node
 *
 */
void traverse_var_decl(ast_var_decl* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_type_name(node->type, pre, post);
    TRACE("is_const: %s", node->is_const? "true": "false");
    TRACE_TERM(node->name);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  var_decl_list
 *      = '(' ( var_decl ( ',' var_decl )* )? ')'
 *
 * @param node
 *
 */
void traverse_var_decl_list(ast_var_decl_list* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    ast_var_decl* nterm;

    init_llist_iter(node->list);
    while(NULL != (nterm = iter_llist(node->list)))
        traverse_var_decl(nterm, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  var_definition
 *      = ( 'var' / 'variable' ) var_decl ( '=' assignment_item )?
 *
 * @param node
 *
 */
void traverse_var_definition(ast_var_definition* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_var_decl(node->type, pre, post);
    TRACE("is_assigned: %s", node->is_assigned? "true": "false");
    if(node->is_assigned)
        traverse_assignment_item(node->item, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

