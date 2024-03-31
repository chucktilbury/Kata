/**
 * @file literals.c
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
 *  scope_operator
 *      = 'private'
 *      / 'public'
 *      / 'protected'
 *
 * @param node
 *
 */
void traverse_scope_operator(ast_scope_operator* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->token);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  literal_type_name
 *      = 'float'
 *      / 'unsigned'
 *      / 'signed'
 *      / 'string'
 *      / 'boolean'
 *      / 'nothing'
 *      / 'list'
 *      / 'dict'
 *      / 'pretion'
 *
 * @param node
 *
 */
void traverse_literal_type_name(ast_literal_type_name* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->token);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  literal_value
 *      = LITERAL_FLOAT
 *      / LITERAL_UNSIGNED
 *      / LITERAL_SIGNED
 *      / LITERAL_BOOL
 *
 * @param node
 *
 */
void traverse_literal_value(ast_literal_value* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->token);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  type_name
 *      = literal_type_name
 *      / compound_name
 *
 * @param node
 *
 */
void traverse_type_name(ast_type_name* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    switch(ast_node_type(node->nterm)) {
        case AST_literal_type_name:
            traverse_literal_type_name((ast_literal_type_name*)node->nterm, pre, post);
            break;
        case AST_compound_name:
            traverse_compound_name((ast_compound_name*)node->nterm, pre, post);
            break;
        default:
            fatal_error("unexpected node type in %s: %s", __func__, nterm_to_str(node->nterm));
    }

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  type_name_list
 *      = '(' ( type_name ( ',' type_name )* )? ')'
 *
 * @param node
 * @param pre
 *
 */
void traverse_type_name_list(ast_type_name_list* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    ast_type_name* nterm;

    void* mark = NULL;
    while(NULL != (nterm = (ast_type_name*)iter_link_list(node->list, &mark)))
        traverse_type_name(nterm, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

void traverse_error(ast_error* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    AST_CALLBACK(post, node);
    RET;
}

