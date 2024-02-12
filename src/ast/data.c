/**
 * @file data.c
 *
 * @brief Implement the functions that travsrse the data AST nodes.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-11-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_ast.h"

struct __ast_type_name__ {
    AstNode node;
    AstNode* elem;
};

struct __ast_type_spec_element__ {
    AstNode node;
    AstNode* elem;
};

struct __ast_type_spec__ {
    AstNode node;
    AstNode* elem;
};

void ast_type_name(AstNode* node);
void ast_type_spec_element(AstNode* node);
void ast_type_spec(AstNode* node);
