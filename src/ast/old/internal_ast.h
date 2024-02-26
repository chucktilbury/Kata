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
#ifndef __INTERNAL_AST_H__
#define __INTERNAL_AST_H__

//#undef USE_TRACE

#include "ast.h"
#include "errors.h"
#include "scan.h"
#include "trace.h"
#include "util.h"

// module.c
void ast_scope_operator(AstScopeOperator* node);
void ast_module_element(AstModuleElement* node);
void ast_module(AstModule* node);
void ast_compound_name(AstCompoundName* node);

// namespace.c
void ast_namespace_element(AstNamespaceElement* node);
void ast_namespace_element_list(AstNamespaceBody* node);
void ast_namespace_definition(AstNamespaceDefinition* node);

// data.c
void ast_type_name(AstTypeName* node);
void ast_type_spec_element(AstTypeSpecElement* node);
void ast_type_spec(AstTypeSpec* node);

// import.c
void ast_import_statement(AstImportStatement* node);

// expression.c
void ast_primary_expression(AstPrimaryExpression* node);
void ast_expression_list(AstExpressionList* node);
void ast_expression(AstExpression* node);
void ast_cast_expression(AstCastExpression* node);

// string.c
void ast_formatted_string(AstFormattedString* node);

#endif /* __INTERNAL_AST_H__ */
