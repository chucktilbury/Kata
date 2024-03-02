/**
 * @file module.c
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

const char* scope_name(ScopeType type);

/**
 * @brief
 *
 *  module
 *      = ( module_item )+
 *
 * @param node
 *
 */
void traverse_module(ast_module* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    PtrListIter* iter = init_ptr_list_iterator(node->list);
    ast_node* nterm;
    while(NULL != (nterm = iterate_ptr_list(iter))) {
        traverse_module_item((ast_module_item*)nterm, func);
    }
    RET;
}

/**
 * @brief
 *
 *  module_item
 *      = namespace_item
 *      / 'import' formatted_strg AS SYMBOL
 *      / start_function
 *
 * @param node
 *
 */
void traverse_module_item(ast_module_item* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    TRACE("SCOPE: %s", scope_name(node->scope));
    switch(ast_node_type(node->nterm)) {
        case AST_namespace_item:
            traverse_namespace_item((ast_namespace_item*)node->nterm, func);
            break;
        case AST_import_statement:
            traverse_import_statement((ast_import_statement*)node->nterm, func);
            break;
        case AST_start_function:
            traverse_start_function((ast_start_function*)node->nterm, func);
            break;
        default:
            RAISE(TRAVERSE_ERROR, "unexpected node type in %s: %d",
                    __func__, ast_node_type(node->nterm));
    }
    RET;
}

/**
 * @brief
 *
 *  namespace_item
 *      = scope_operator
 *      / namespace_definition
 *      / class_definition
 *      / func_definition
 *      / ( 'const' )? var_definition
 *
 * @param node
 *
 */
void traverse_namespace_item(ast_namespace_item* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    TRACE("SCOPE: %s", scope_name(node->scope));
    switch(ast_node_type(node->nterm)) {
        case AST_scope_operator:
            traverse_scope_operator((ast_scope_operator*)node->nterm, func);
            break;
        case AST_func_definition:
            traverse_func_definition((ast_func_definition*)node->nterm, func);
            break;
        case AST_namespace_definition:
            traverse_namespace_definition((ast_namespace_definition*)node->nterm, func);
            break;
        case AST_class_definition:
            traverse_class_definition((ast_class_definition*)node->nterm, func);
            break;
        case AST_var_definition:
            traverse_var_definition((ast_var_definition*)node->nterm, func);
            break;
        default:
            RAISE(TRAVERSE_ERROR, "unexpected node type in %s: %d",
                    __func__, ast_node_type(node->nterm));
    }
    RET;
}

/**
 * @brief
 *
 *  namespace_definition
 *      = 'namespace' SYMBOL '{' ( namespace_item )+ '}'
 *
 * @param node
 *
 */
void traverse_namespace_definition(ast_namespace_definition* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    TRACE("NAME: %s", raw_string(node->name->str));
    TRACE("SCOPE: %s", scope_name(node->scope));
    PtrListIter* iter = init_ptr_list_iterator(node->list);
    ast_node* nterm;
    while(NULL != (nterm = iterate_ptr_list(iter)))
        traverse_namespace_item((ast_namespace_item*)nterm, func);
    RET;
}

/**
 * @brief
 *
 *  class_item
 *      = scope_operator
 *      / var_decl
 *      / func_decl
 *
 * @param node
 *
 */
void traverse_class_item(ast_class_item* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    TRACE("SCOPE: %s", scope_name(node->scope));
    switch(ast_node_type(node->nterm)) {
        case AST_scope_operator:
            traverse_scope_operator((ast_scope_operator*)node->nterm, func);
            break;
        case AST_var_decl:
            traverse_var_decl((ast_var_decl*)node->nterm, func);
            break;
        case AST_func_decl:
            traverse_func_decl((ast_func_decl*)node->nterm, func);
            break;
        default:
            RAISE(TRAVERSE_ERROR, "unexpected node type in %s: %d",
                    __func__, ast_node_type(node->nterm));
    }
    RET;
}

/**
 * @brief
 *
 *  class_definition
 *      = 'class' SYMBOL ( '(' ( type_name )? ')' )?
 *              '{' ( class_item )+ '}'
 *
 * @param node
 *
 */
void traverse_class_definition(ast_class_definition* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    TRACE("NAME: %s", raw_string(node->name->str));
    TRACE("SCOPE: %s", scope_name(node->scope));
    if(node->parent)
        traverse_type_name(node->parent, func);
    PtrListIter* iter = init_ptr_list_iterator(node->list);
    ast_node* nterm;
    while(NULL != (nterm = iterate_ptr_list(iter)))
        traverse_class_item((ast_class_item*)nterm, func);
    RET;
}


