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
void traverse_module(ast_module* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    ast_node* nterm;

    init_llist_iter(node->list);
    while(NULL != (nterm = iter_llist(node->list))) {
        traverse_module_item((ast_module_item*)nterm, pre, post);
    }

    AST_CALLBACK(post, node);

    AST_CALLBACK(post, node);
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
void traverse_module_item(ast_module_item* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE("SCOPE: %s", scope_name(node->scope));
    switch(ast_node_type(node->nterm)) {
        case AST_namespace_item:
            traverse_namespace_item((ast_namespace_item*)node->nterm, pre, post);
            break;
        case AST_import_statement:
            traverse_import_statement((ast_import_statement*)node->nterm, pre, post);
            break;
        case AST_start_function:
            traverse_start_function((ast_start_function*)node->nterm, pre, post);
            break;
        default:
            RAISE(TRAVERSE_ERROR, "unexpected node type in %s: %d",
                    __func__, ast_node_type(node->nterm));
    }

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  namespace_item
 *      = scope_operator
 *      / namespace_definition
 *      / class_definition
 *      / function_definition
 *      / create_definition
 *      / destroy_definition
 *      / var_definition
 *
 * @param node
 *
 */
void traverse_namespace_item(ast_namespace_item* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE("SCOPE: %s", scope_name(node->scope));
    switch(ast_node_type(node->nterm)) {
        case AST_scope_operator:
            traverse_scope_operator((ast_scope_operator*)node->nterm, pre, post);
            break;
        case AST_function_definition:
            traverse_function_definition((ast_function_definition*)node->nterm, pre, post);
            break;
        case AST_create_definition:
            traverse_create_definition((ast_create_definition*)node->nterm, pre, post);
            break;
        case AST_destroy_definition:
            traverse_destroy_definition((ast_destroy_definition*)node->nterm, pre, post);
            break;
        case AST_namespace_definition:
            traverse_namespace_definition((ast_namespace_definition*)node->nterm, pre, post);
            break;
        case AST_class_definition:
            traverse_class_definition((ast_class_definition*)node->nterm, pre, post);
            break;
        case AST_var_definition:
            traverse_var_definition((ast_var_definition*)node->nterm, pre, post);
            break;
        default:
            RAISE(TRAVERSE_ERROR, "unexpected node type in %s: %d",
                    __func__, ast_node_type(node->nterm));
    }

    AST_CALLBACK(post, node);
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
void traverse_namespace_definition(ast_namespace_definition* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE("NAME: %s", raw_string(node->name->str));
    TRACE("SCOPE: %s", scope_name(node->scope));
    ast_node* nterm;

    init_llist_iter(node->list);
    while(NULL != (nterm = iter_llist(node->list)))
        traverse_namespace_item((ast_namespace_item*)nterm, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  class_item
 *      = scope_operator
 *      / var_decl
 *      / function_declaration
 *      / create_declaration
 *      / destroy_declaration
 *
 * @param node
 *
 */
void traverse_class_item(ast_class_item* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE("SCOPE: %s", scope_name(node->scope));
    switch(ast_node_type(node->nterm)) {
        case AST_scope_operator:
            traverse_scope_operator((ast_scope_operator*)node->nterm, pre, post);
            break;
        case AST_class_var_declaration:
            traverse_class_var_declaration((ast_class_var_declaration*)node->nterm, pre, post);
            break;
        case AST_function_declaration:
            traverse_function_declaration((ast_function_declaration*)node->nterm, pre, post);
            break;
        case AST_create_declaration:
            traverse_create_declaration((ast_create_declaration*)node->nterm, pre, post);
            break;
        case AST_destroy_declaration:
            traverse_destroy_declaration((ast_destroy_declaration*)node->nterm, pre, post);
            break;
        default:
            RAISE(TRAVERSE_ERROR, "unexpected node type in %s: %d",
                    __func__, ast_node_type(node->nterm));
    }

    AST_CALLBACK(post, node);
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
void traverse_class_definition(ast_class_definition* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE("NAME: %s", raw_string(node->name->str));
    TRACE("SCOPE: %s", scope_name(node->scope));
    if(node->parent)
        traverse_type_name(node->parent, pre, post);

    ast_node* nterm;

    init_llist_iter(node->list);
    while(NULL != (nterm = iter_llist(node->list)))
        traverse_class_item((ast_class_item*)nterm, pre, post);

    AST_CALLBACK(post, node);
    RET;
}


void traverse_class_var_declaration(ast_class_var_declaration* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_var_decl(node->var, pre, post);

    AST_CALLBACK(post, node);
    RET;
}
