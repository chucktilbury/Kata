/**
 * @file internal_ast.h
 *
 * @brief This is an internal header that forward-declares functions for
 * the recursive descent AST traverse.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-09-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _INTERNAL_AST_H
#define _INTERNAL_AST_H

#include "ast.h"
#include "errors.h"
#include "scanner.h"
#include "trace.h"
#include "util.h"

// Defined in module.c
void ast_scope_operator(AstNode* node);
void ast_module_element(AstNode* node);
void ast_module(AstNode* node);
void ast_compound_name(AstNode* node);

// namespace.c
void ast_namespace_element(AstNode* node);
void ast_namespace_element_list(AstNode* node);
void ast_namespace_definition(AstNode* node);

// Defined in data.c
void ast_type_name(AstNode* node);
void ast_type_spec_element(AstNode* node);
void ast_type_spec(AstNode* node);

// Defined in import.c
void ast_import_statement(AstNode* node);

// Defined in expression.c
void ast_primary_expression(AstNode* node);
void ast_expression_list(AstNode* node);
void ast_expression(AstNode* node);
void ast_cast_statement(AstNode* node);

#endif /* _INTERNAL_AST_H */
