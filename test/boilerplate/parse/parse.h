/**
 * @file parse.h
 * 
 * @brief 
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __PARSE_H__
#define __PARSE_H__

#include "ast.h"

// module.c
ast_module* parse_module();
ast_module_item* parse_module_item();
ast_namespace_item* parse_namespace_item();
ast_namespace_definition* parse_namespace_definition();
ast_class_item* parse_class_item();
ast_class_definition* parse_class_definition();

// compound.c
ast_compound_name* parse_compound_name();
ast_compound_ref_item* parse_compound_ref_item();
ast_compound_reference* parse_compound_reference();

// literals.c
ast_scope_operator* parse_scope_operator();
ast_literal_type_name* parse_literal_type_name();
ast_literal_value* parse_literal_value();
ast_type_name* parse_type_name();
ast_formatted_strg* parse_formatted_strg();

// string.c
ast_string_literal* parse_string_literal();
ast_string_expr_item* parse_string_expr_item();
ast_string_expr* parse_string_expr();

// vars.c
ast_var_decl* parse_var_decl();
ast_var_decl_list* parse_var_decl_list();
ast_var_definition* parse_var_definition();

// list.c
ast_list_init* parse_list_init();
ast_dict_init_element* parse_dict_init_element();
ast_dict_init* parse_dict_init();
ast_array_param* parse_array_param();
ast_array_reference* parse_array_reference();

// func.c
ast_function_reference* parse_function_reference();
ast_func_decl* parse_func_decl();
ast_func_definition* parse_func_definition();
ast_function_body* parse_function_body();
ast_function_body_element* parse_function_body_element();

// func_body.c
ast_break_statement* parse_break_statement();
ast_continue_statement* parse_continue_statement();
ast_trace_statement* parse_trace_statement();
ast_inline_statement* parse_inline_statement();
ast_yield_statement* parse_yield_statement();
ast_type_statement* parse_type_statement();
ast_exit_statement* parse_exit_statement();
ast_print_statement* parse_print_statement();
ast_return_statement* parse_return_statement();
ast_raise_statement* parse_raise_statement();

// expr.c
ast_expression* parse_expression();
ast_expr_primary* parse_expr_primary();
ast_expression_list* parse_expression_list();
ast_assignment_item* parse_assignment_item();
ast_assignment* parse_assignment();

// loop.c
ast_while_definition* parse_while_definition();
ast_while_clause* parse_while_clause();
ast_do_clause* parse_do_clause();
ast_for_clause* parse_for_clause();

// flow.c
ast_if_clause* parse_if_clause();
ast_else_clause_mid* parse_else_clause_mid();
ast_else_clause_final* parse_else_clause_final();
ast_else_clause* parse_else_clause();
ast_switch_clause* parse_switch_clause();
ast_case_clause* parse_case_clause();
ast_default_clause* parse_default_clause();
ast_case_body* parse_case_body();

// except.c
ast_try_clause* parse_try_clause();
ast_except_clause_mid* parse_except_clause_mid();
ast_except_clause_final* parse_except_clause_final();
ast_except_clause* parse_except_clause();

// parse.c
void init_parser(const char* fname);
ast_module* parse();

#endif /* __PARSE_H__ */
