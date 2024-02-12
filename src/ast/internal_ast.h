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
int ast_scope_operator(AstScopeOperator* node);
int ast_module_element(AstModuleElement* node);
int ast_module(AstModule* node);
int ast_compound_name(AstCompoundName* node);

// namespace.c
int ast_namespace_element(AstNamespaceElement* node);
int ast_namespace_element_list(AstNamespaceBody* node);
int ast_namespace_definition(AstNamespaceDefinition* node);

// Defined in data.c
int ast_type_name(AstTypeName* node);
int ast_type_spec_element(AstTypeSpecElement* node);
int ast_type_spec(AstTypeSpec* node);

// Defined in import.c
int ast_import_statement(AstImportStatement* node);

// Defined in expression.c
int ast_primary_expression(AstPrimaryExpression* node);
int ast_expression_list(AstExpressionList* node);
int ast_expression(AstExpression* node);
int ast_cast_expression(AstCastExpression* node);

#endif /* _INTERNAL_AST_H */
