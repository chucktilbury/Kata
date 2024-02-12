/**
 * @file expression.c
 *
 * @brief Implement the functions that traverse the expressions AST nodes.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-11-2024
 * @copyright Copyright (c) 2024
 */

#include "internal_ast.h"

struct __ast_primary_expression__ {
    AstNode node;
    AstNode* elem;
};

struct __ast_expression_list__ {
    AstNode node;
    List* lst;
};

struct __ast_expression__ {
    AstNode node;
    AstNode* elem;
};

struct __ast_cast_statement__ {
    AstNode node;
    AstNode* elem;
};


void ast_primary_expression(AstNode* node);
void ast_expression_list(AstNode* node);
void ast_expression(AstNode* node);
void ast_cast_statement(AstNode* node);
