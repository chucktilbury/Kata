/**
 * @file internal_parser.h
 *
 * @brief This is an internal header that forward-declares functions for the
 * recursive descent parser.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-09-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _INTERNAL_PARSER_H
#define _INTERNAL_PARSER_H
#include "util.h"
#include "scanner.h"
#include "errors.h"
#include "trace.h"
#include "ast.h"

// Defined in parser.c
void pop_namespace();
void push_namespace(Str* str);
Str* peek_namespace();

// Defined in module.c
AstNode* scope_operator();
AstNode* type_name();
AstNode* compound_name();
AstNode* type_spec_element();
AstNode* type_spec();
AstNode* namespace_element();
AstNode* namespace_element_list();
AstNode* namespace_definition();
AstNode* module_element();
AstNode* module();

// Defined in import.c
AstNode* import_statement();


#endif /* _INTERNAL_PARSER_H */
