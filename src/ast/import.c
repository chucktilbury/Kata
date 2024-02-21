/**
 * @file import.c
 *
 * @brief Implement the functions that traverse the AST.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-11-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_ast.h"

/**
 * import_statement
 *      = 'import' STR 'as' SYMBOL
 *
 * @param node
 *
 */
void ast_import_statement(AstImportStatement* node) {

    assert(node != NULL);
    assert(node->node.type == AST_import_statement);

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));
    TRACE_TERM(node->fname);
    TRACE_TERM(node->symbol);
    RET;
}
