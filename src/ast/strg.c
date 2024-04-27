/**
 * @file string.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#include <assert.h>

#include "ast.h"
#include "errors.h"
#include "trace.h"

/**
 * @brief
 *
 *  formatted_strg
 *      = LITERAL_DSTRG (expression_list)?
 *
 * @param node
 *
 */
void traverse_formatted_strg(ast_formatted_strg* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->str);
    traverse_string_expr_list(node->exprs, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  string_literal
 *      = LITERAL_SSTRG
 *      / formatted_strg
 *
 * @param node
 *
 */
void traverse_string_literal(ast_string_literal* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);
    if(node->literal != NULL)
        TRACE_TERM(node->literal);
    else
        traverse_formatted_strg(node->fmt, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  string_expr_item
 *      = string_literal
 *      / compound_reference
 *      / literal_value
 *
 * @param node
 *
 */
void traverse_string_expr_item(ast_string_expr_item* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    switch(ast_node_type(node->nterm)) {
        case AST_string_literal:
            traverse_string_literal((ast_string_literal*)node->nterm, pre, post);
            break;
        case AST_compound_reference:
            traverse_compound_reference((ast_compound_reference*)node->nterm, pre, post);
            break;
        case AST_literal_value:
            traverse_literal_value((ast_literal_value*)node->nterm, pre, post);
            break;
        default:
            fatal_error("unexpected node type in %s: %d", __func__,
                        ast_node_type(node->nterm));
    }

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  string_expr_list
 *      = '(' string_expr_item ( ',' string_expr_item )* ')'
 *
 * @param node
 *
 */
void traverse_string_expr_list(ast_string_expr_list* node, PassFunc pre, PassFunc post) {

    ENTER;
    // It's possible that there is no expr list following the string
    if(node != NULL) {
        AST_CALLBACK(pre, node);

        ast_string_expr_item* item;

        TRACE("num expr items: %d", len_link_list(node->list));
        void* mark = NULL;
        while(NULL != (item = (ast_string_expr_item*)iter_link_list(node->list, &mark)))
            traverse_string_expr_item(item, pre, post);

        AST_CALLBACK(post, node);
    }
    RET;
}
