/**
 * @file list.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#include <assert.h>

#include "trace.h"
#include "ast.h"
#include "errors.h"

/**
 * @brief
 *
 *  list_init
 *      = '[' assignment_item ( ',' assignment_item )* ']'
 *
 * @param node
 *
 */
void traverse_list_init(ast_list_init* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    void* mark = NULL;
    ast_assignment_item* item;
    while(NULL != (item = iter_link_list(node->list, &mark)))
        traverse_assignment_item(item, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  dict_init_element
 *      = ( LITERAL_DSTRG / LITERAL_SSTRG ) ':' assignment_item
 *
 * @param node
 *
 */
void traverse_dict_init_element(ast_dict_init_element* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->key);
    traverse_assignment_item(node->item, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  dict_init
 *      = '[' dict_init_element ( ',' dict_init_element )* ']'
 *
 * @param node
 *
 */
void traverse_dict_init(ast_dict_init* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    void* mark = NULL;
    ast_dict_init_element* elem;
    while(NULL != (elem = iter_link_list(node->list, &mark)))
        traverse_dict_init_element(elem, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  array_param
 *      = '[' ( expression / string_expression ) ']'
 *
 * @param node
 *
 */
void traverse_array_param(ast_array_param* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    switch(ast_node_type(node->nterm)) {
        case AST_expression:
            traverse_expression((ast_expression*)node->nterm, pre, post);
            break;
        case AST_string_expr:
            traverse_string_expr((ast_string_expr*)node->nterm, pre, post);
            break;
        default:
            fatal_error("unexpected state in %s: %d", __func__, ast_node_type(node->nterm));
    }

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  array_param_list
 *      = array_param ( array_param )*
 *
 * @param node
 *
 */
void traverse_array_param_list(ast_array_param_list* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    void* mark = NULL;
    ast_array_param* item;
    while(NULL != (item = iter_link_list(node->list, &mark)))
        traverse_array_param(item, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  array_reference
 *      = SYMBOL array_param_list
 *
 * @param node
 *
 */
void traverse_array_reference(ast_array_reference* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->symbol);
    traverse_array_param_list(node->param, pre, post);

    AST_CALLBACK(post, node);
    RET;
}


