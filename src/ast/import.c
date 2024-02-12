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

int ast_import_statement(AstImportStatement* node) {

    assert(node != NULL);
    assert(node->node.type == AST_import_statement);

    ENTER;
    RETV(0);
}
