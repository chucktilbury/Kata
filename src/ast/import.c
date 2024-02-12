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

struct __ast_import_statement__ {
    AstNode node;
    AstNode* elem;
};

void ast_import_statement(AstNode* node);
