/**
 * @file string.c
 *
 * @brief Traverse the AST formatted string
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-20-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_ast.h"

void ast_formatted_string(AstFormattedString* node) {

    assert(node != NULL);
    assert(AST_formatted_string == get_ast_node_type((AstNode*)node));

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));
    TRACE_TERM(node->strg);
    ast_expression_list(node->expr_lst);

    RET;
}