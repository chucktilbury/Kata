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
    traverse_expression_list(node->exprs, pre, post);

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
