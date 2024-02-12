/**
 * @file namespace.c
 *
 * @brief Namespace AST functions
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-11-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_ast.h"

struct __ast_namespace_element__ {
    AstNode node;
    AstNode* elem;
};

struct __ast_namespace_element_list__ {
    AstNode node;
    List* lst;
};

struct __ast_namespace_definition__ {
    AstNode node;
    AstNamespaceDefinition* def;
};


void ast_namespace_element(AstNode* node);
void ast_namespace_element_list(AstNode* node);
void ast_namespace_definition(AstNode* node);
