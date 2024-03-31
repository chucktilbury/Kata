/**
 * @file import.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-26-2024
 * @copyright Copyright (c) 2024
 */
#include <assert.h>

#include "trace.h"
#include "ast.h"

/**
 * @brief
 *
 *  import_statement
 *      = 'import' SYMBOL ('as' SYMBOL)?
 *
 * @param node
 *
 */
void traverse_import_statement(ast_import_statement* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->name);
    TRACE_TERM(node->mod);
    traverse_module(node->module, pre, post);

    AST_CALLBACK(post, node);
    RET;
}
