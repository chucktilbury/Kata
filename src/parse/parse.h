/**
 * @file parse.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _PARSE_H_
#define _PARSE_H_

#include "ast.h"
#include "errors.h"
#include "parse_state.h"

// module.c
ast_module* parse_module(void);
ast_module_item* parse_module_item(void);
ast_namespace_item* parse_namespace_item(void);
ast_namespace_definition* parse_namespace_definition(void);
ast_class_item* parse_class_item(void);
ast_class_definition* parse_class_definition(void);
ast_class_var_declaration* parse_class_var_declaration(void);

// compound.c
ast_compound_name* parse_compound_name(void);
ast_compound_name_list* parse_compound_name_list(void);
ast_compound_ref_item* parse_compound_ref_item(void);
ast_compound_reference* parse_compound_reference(void);

// literals.c
ast_scope_operator* parse_scope_operator(void);
ast_literal_type_name* parse_literal_type_name(void);
ast_literal_value* parse_literal_value(void);
ast_type_name* parse_type_name(void);
ast_type_name_list* parse_type_name_list(void);

// strg.c
ast_string_literal* parse_string_literal(void);
ast_formatted_strg* parse_formatted_strg(void);

// vars.c
ast_var_decl* parse_var_decl(void);
ast_var_decl_list* parse_var_decl_list(void);
ast_var_definition* parse_var_definition(void);

// list.c
ast_list_init* parse_list_init(void);
ast_dict_init_element* parse_dict_init_element(void);
//ast_dict_init* parse_dict_init(void);
ast_array_param* parse_array_param(void);
ast_array_param_list* parse_array_param_list(void);
ast_array_reference* parse_array_reference(void);

// func.c
ast_function_reference* parse_function_reference(void);
ast_create_reference* parse_create_reference(void);
ast_destroy_reference* parse_destroy_reference(void);
ast_function_declaration* parse_function_declaration(void);
ast_create_declaration* parse_create_declaration(void);
ast_destroy_declaration* parse_destroy_declaration(void);
ast_function_definition* parse_function_definition(void);
ast_create_name* parse_create_name(void);
ast_destroy_name* parse_destroy_name(void);
ast_create_definition* parse_create_definition(void);
ast_destroy_definition* parse_destroy_definition(void);
ast_function_body* parse_function_body(void);
ast_start_function* parse_start_function(void);
ast_function_assignment* parse_function_assignment(void);

// func_body.c
ast_function_body_element* parse_function_body_element(void);
ast_break_statement* parse_break_statement(void);
ast_continue_statement* parse_continue_statement(void);
ast_inline_statement* parse_inline_statement(void);
ast_yield_statement* parse_yield_statement(void);
ast_type_statement* parse_type_statement(void);
ast_return_statement* parse_return_statement(void);
ast_raise_statement* parse_raise_statement(void);
ast_trace_statement* parse_trace_statement(void);
ast_print_statement* parse_print_statement(void);
ast_exit_statement* parse_exit_statement(void);

// expr.c
ast_expression_list* parse_expression_list(void);
ast_assignment_item* parse_assignment_item(void);
ast_assignment* parse_assignment(void);
ast_cast_statement* parse_cast_statement(void);

// expression.c
ast_operator* parse_operator(void);
ast_expression* parse_expression(void);
ast_expr_primary* parse_expr_primary(void);

// loop.c
ast_while_definition* parse_while_definition(void);
ast_while_clause* parse_while_clause(void);
ast_do_clause* parse_do_clause(void);
ast_for_clause* parse_for_clause(void);

// flow.c
ast_if_clause* parse_if_clause(void);
ast_elif_clause* parse_elif_clause(void);
ast_else_clause* parse_else_clause(void);

// except.c
ast_try_clause* parse_try_clause(void);
ast_except_clause* parse_except_clause(void);
ast_final_clause* parse_final_clause(void);

// import.c
ast_import_statement* parse_import_statement(void);

// parse.c
void init_parser(const char* fname);
ast_module* parse(void);

#define PARSER_SYNTAX_ERROR 1000

#endif  /* _PARSE_H_ */
