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
void traverse_var_decl(ast_var_decl* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
    traverse_type_name(node->type, func);
    TRACE("is_const: %s", node->is_const? "true": "false");
    TRACE_TERM(node->name);
    RET;
}

/**
 * @brief 
 * 
 *  var_decl_list
 *      = var_decl ( ',' var_decl )*
 *      
 * @param node 
 * 
 */
void traverse_var_decl_list(ast_var_decl_list* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
    ast_var_decl* nterm;

    init_llist_iter(node->list);
    while(NULL != (nterm = iter_llist(node->list)))
        traverse_var_decl(nterm, func);
    RET;
}

/**
 * @brief 
 * 
 *  var_definition
 *      = var_decl ( '=' assignment_item )?
 *      
 * @param node 
 * 
 */
void traverse_var_definition(ast_var_definition* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
    traverse_var_decl(node->type, func);
    TRACE("is_assigned: %s", node->is_assigned? "true": "false");
    if(node->is_assigned) 
        traverse_assignment_item(node->item, func);
    RET;
}

