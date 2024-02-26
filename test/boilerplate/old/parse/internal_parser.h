/**
 * @file internal_parser.h
 *
 * @brief This is an internal header that forward-declares functions for
 * the recursive descent parser.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-09-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _INTERNAL_PARSER_H
#define _INTERNAL_PARSER_H

//#undef USE_TRACE

#include "ast.h"
#include "errors.h"
#include "scanner.h"
#include "trace.h"
#include "util.h"

// Parser Exceptions
typedef enum {
    SYNTAX_ERROR = 0x8030,
    AST_ERROR,
    AST_NF_ERROR,
    AST_DUP_ERROR,
    MISC_ERROR,
} ParserException;

// parser.c
void pop_namespace();
void push_namespace(Str* str);
Str* peek_namespace();

// module.c
AstCompoundName* parse_compound_name();
AstScopeOperator* parse_scope_operator();
AstModuleElement* parse_module_element();
AstModule* parse_module();

// namespace.c
AstNamespaceElement* parse_namespace_element();
AstNamespaceBody* parse_namespace_body();
AstNamespaceDefinition* parse_namespace_definition();

// data.c
AstTypeName* parse_type_name();
AstTypeSpecElement* parse_type_spec_element();
AstTypeSpec* parse_type_spec();

// Defined in import.c
AstImportStatement* parse_import_statement();

// Defined in expression.c
AstPrimaryExpression* parse_primary_expression();
AstExpressionList* parse_expression_list();
AstExpression* parse_expression();
AstCastExpression* parse_cast_expression();

// string.c
AstFormattedString* parse_formatted_string();

#endif /* _INTERNAL_PARSER_H */
