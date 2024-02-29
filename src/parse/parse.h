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
#include "errors.h"

/**
 * @brief This data structure is a stub to template passing a state to parse 
 * funcitons.
 */
typedef struct {
    TokenType scope;
} ParserState;

// module.c
ast_module* parse_module(ParserState* state);
ast_module_item* parse_module_item(ParserState* state);
ast_namespace_item* parse_namespace_item(ParserState* state);
ast_namespace_definition* parse_namespace_definition(ParserState* state);
ast_class_item* parse_class_item(ParserState* state);
ast_class_definition* parse_class_definition(ParserState* state);

// compound.c
ast_compound_name* parse_compound_name(ParserState* state);
ast_compound_ref_item* parse_compound_ref_item(ParserState* state);
ast_compound_reference* parse_compound_reference(ParserState* state);

// literals.c
ast_scope_operator* parse_scope_operator(ParserState* state);
ast_literal_type_name* parse_literal_type_name(ParserState* state);
ast_literal_value* parse_literal_value(ParserState* state);
ast_type_name* parse_type_name(ParserState* state);

// string.c
ast_string_literal* parse_string_literal(ParserState* state);
ast_string_expr_item* parse_string_expr_item(ParserState* state);
ast_string_expr* parse_string_expr(ParserState* state);
ast_formatted_strg* parse_formatted_strg(ParserState* state);

// vars.c
ast_var_decl* parse_var_decl(ParserState* state);
ast_var_decl_list* parse_var_decl_list(ParserState* state);
ast_var_definition* parse_var_definition(ParserState* state);

// list.c
ast_list_init* parse_list_init(ParserState* state);
ast_dict_init_element* parse_dict_init_element(ParserState* state);
ast_dict_init* parse_dict_init(ParserState* state);
ast_array_param* parse_array_param(ParserState* state);
ast_array_reference* parse_array_reference(ParserState* state);

// func.c
ast_function_reference* parse_function_reference(ParserState* state);
ast_func_decl* parse_func_decl(ParserState* state);
ast_func_definition* parse_func_definition(ParserState* state);
ast_function_body* parse_function_body(ParserState* state);
ast_start_function* parse_start_function(ParserState* state);

// func_body.c
ast_function_body_element* parse_function_body_element(ParserState* state);
ast_break_statement* parse_break_statement(ParserState* state);
ast_continue_statement* parse_continue_statement(ParserState* state);
ast_trace_statement* parse_trace_statement(ParserState* state);
ast_inline_statement* parse_inline_statement(ParserState* state);
ast_yield_statement* parse_yield_statement(ParserState* state);
ast_type_statement* parse_type_statement(ParserState* state);
ast_exit_statement* parse_exit_statement(ParserState* state);
ast_print_statement* parse_print_statement(ParserState* state);
ast_return_statement* parse_return_statement(ParserState* state);
ast_raise_statement* parse_raise_statement(ParserState* state);

// expr.c
ast_expression* parse_expression(ParserState* state);
ast_expr_primary* parse_expr_primary(ParserState* state);
ast_expression_list* parse_expression_list(ParserState* state);
ast_assignment_item* parse_assignment_item(ParserState* state);
ast_assignment* parse_assignment(ParserState* state);

// loop.c
ast_while_definition* parse_while_definition(ParserState* state);
ast_while_clause* parse_while_clause(ParserState* state);
ast_do_clause* parse_do_clause(ParserState* state);
ast_for_clause* parse_for_clause(ParserState* state);

// flow.c
ast_if_clause* parse_if_clause(ParserState* state);
ast_else_clause_mid* parse_else_clause_mid(ParserState* state);
ast_else_clause_final* parse_else_clause_final(ParserState* state);
ast_else_clause* parse_else_clause(ParserState* state);
ast_switch_clause* parse_switch_clause(ParserState* state);
ast_case_clause* parse_case_clause(ParserState* state);
ast_default_clause* parse_default_clause(ParserState* state);
ast_case_body* parse_case_body(ParserState* state);

// except.c
ast_try_clause* parse_try_clause(ParserState* state);
ast_except_clause_mid* parse_except_clause_mid(ParserState* state);
ast_except_clause_final* parse_except_clause_final(ParserState* state);
ast_except_clause* parse_except_clause(ParserState* state);

// import.c
ast_import_statement* parse_import_statement(ParserState* state);

// parse.c
ParserState* init_parser(const char* fname);
ast_module* parse(ParserState* state);

#endif /* __PARSE_H__ */
