/**
 * @file module.c
 *
 * @brief Implement the functions that traverse the AST.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-11-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_ast.h"

struct __ast_module__ {
    AstNode node;
    List* lst;
};

struct __ast_scope_operator__ {
    AstNode node;
    Token* token;
};

struct __ast_module_element__ {
    AstNode node;
    AstNode* elem;
};

struct __ast_compound_name__ {
    AstNode node;
    List* lst;
};

void ast_scope_operator(AstNode* node);
void ast_module_element(AstNode* node);
void ast_module(AstNode* node);
void ast_compound_name(AstNode* node);
