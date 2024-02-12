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

// Defined in parser.c
void pop_namespace();
void push_namespace(Str* str);
Str* peek_namespace();

// Defined in module.c
AstNode* parse_scope_operator();
AstNode* parse_namespace_element();
AstNode* parse_namespace_element_list();
AstNode* parse_namespace_definition();
AstNode* parse_module_element();
AstNode* parse_module();

// Defined in data.c
AstNode* parse_type_name();
AstNode* parse_compound_name();
AstNode* parse_type_spec_element();
AstNode* parse_type_spec();

// Defined in import.c
AstNode* parse_import_statement();

// Defined in expression.c
AstNode* parse_primary_expression();
AstNode* parse_expression_list();
AstNode* parse_expression();
AstNode* parse_cast_statement();


#endif /* _INTERNAL_PARSER_H */
