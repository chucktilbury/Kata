/**
 * @file pass.c
 *
 * @brief This is a template of an AST pass. It implements a single funciton
 * that uses TRACE to show that it was executed.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-27-2024
 * @copyright Copyright (c) 2024
 */
#include "trace.h"
#include "parse.h"
#include "trace.h"


/**
 * @brief Simple function that simply prints the node type and the number
 * to demonstrate that the function as actually called.
 *
 * @param node
 * @param number
 *
 */
void pre_test_pass_func(ast_node* node, int number) {

    ENTER;
    TRACE("node type: %s(%d)", nterm_to_str(node), number);
    RET;
}

void post_test_pass_func(ast_node* node, int number) {

    ENTER;
    TRACE("node type: %s(%d)", nterm_to_str(node), number);
    RET;
}

/**
 * @brief This function specifically does nothing. It is called when none of
 * the nodes are a match in the pre_test_pass().
 */
void dummy_function() {

    ENTER;
    TRACE("dummy function");
    RET;
}

/**
 * @brief This function calls the test pass function. This way, there can be
 * only the elements that are active in the array. There must be a dummy
 * function that does nothing if a node is entered without a handler. There
 * are no actual restrictions on the signature of the function called, but
 * all of the information needed by the pass function must be provided in the
 * AST node. This function should be used as a template for other passes.
 *
 * @param node
 * @param type
 * @return * void
 *
 */
void pre_test_pass(ast_node* node) {

    AstType type = ast_node_type(node);

    (type == AST_module)? pre_test_pass_func(node, AST_module) :
    (type == AST_module_item)? pre_test_pass_func(node, AST_module_item) :
    (type == AST_namespace_item)? pre_test_pass_func(node, AST_namespace_item) :
    (type == AST_namespace_definition)? pre_test_pass_func(node, AST_namespace_definition) :
    (type == AST_class_item)? pre_test_pass_func(node, AST_class_item) :
    (type == AST_class_definition)? pre_test_pass_func(node, AST_class_definition) :
    (type == AST_compound_name)? pre_test_pass_func(node, AST_compound_name) :
    (type == AST_compound_ref_item)? pre_test_pass_func(node, AST_compound_ref_item) :
    (type == AST_compound_reference)? pre_test_pass_func(node, AST_compound_reference) :
    (type == AST_scope_operator)? pre_test_pass_func(node, AST_scope_operator) :
    (type == AST_literal_type_name)? pre_test_pass_func(node, AST_literal_type_name) :
    (type == AST_literal_value)? pre_test_pass_func(node, AST_literal_value) :
    (type == AST_type_name)? pre_test_pass_func(node, AST_type_name) :
    (type == AST_formatted_strg)? pre_test_pass_func(node, AST_formatted_strg) :
    (type == AST_string_literal)? pre_test_pass_func(node, AST_string_literal) :
    (type == AST_string_expr_item)? pre_test_pass_func(node, AST_string_expr_item) :
    (type == AST_string_expr)? pre_test_pass_func(node, AST_string_expr) :
    (type == AST_var_decl)? pre_test_pass_func(node, AST_var_decl) :
    (type == AST_var_decl_list)? pre_test_pass_func(node, AST_var_decl_list) :
    (type == AST_var_definition)? pre_test_pass_func(node, AST_var_definition) :
    (type == AST_list_init)? pre_test_pass_func(node, AST_list_init) :
    (type == AST_dict_init_element)? pre_test_pass_func(node, AST_dict_init_element) :
    (type == AST_dict_init)? pre_test_pass_func(node, AST_dict_init) :
    (type == AST_array_param)? pre_test_pass_func(node, AST_array_param) :
    (type == AST_array_reference)? pre_test_pass_func(node, AST_array_reference) :
    (type == AST_function_reference)? pre_test_pass_func(node, AST_function_reference) :
    (type == AST_create_reference)? pre_test_pass_func(node, AST_create_reference) :
    (type == AST_destroy_reference)? pre_test_pass_func(node, AST_destroy_reference) :
    (type == AST_function_declaration)? pre_test_pass_func(node, AST_function_declaration) :
    (type == AST_create_declaration)? pre_test_pass_func(node, AST_create_declaration) :
    (type == AST_destroy_declaration)? pre_test_pass_func(node, AST_destroy_declaration) :
    (type == AST_function_definition)? pre_test_pass_func(node, AST_function_definition) :
    (type == AST_create_name)? pre_test_pass_func(node, AST_create_name) :
    (type == AST_destroy_name)? pre_test_pass_func(node, AST_destroy_name) :
    (type == AST_create_definition)? pre_test_pass_func(node, AST_create_definition) :
    (type == AST_destroy_definition)? pre_test_pass_func(node, AST_destroy_definition) :
    (type == AST_function_body)? pre_test_pass_func(node, AST_function_body) :
    (type == AST_function_body_element)? pre_test_pass_func(node, AST_function_body_element) :
    (type == AST_expression)? pre_test_pass_func(node, AST_expression) :
    (type == AST_expr_primary)? pre_test_pass_func(node, AST_expr_primary) :
    (type == AST_expression_list)? pre_test_pass_func(node, AST_expression_list) :
    (type == AST_assignment_item)? pre_test_pass_func(node, AST_assignment_item) :
    (type == AST_assignment)? pre_test_pass_func(node, AST_assignment) :
    (type == AST_while_definition)? pre_test_pass_func(node, AST_while_definition) :
    (type == AST_while_clause)? pre_test_pass_func(node, AST_while_clause) :
    (type == AST_do_clause)? pre_test_pass_func(node, AST_do_clause) :
    (type == AST_for_clause)? pre_test_pass_func(node, AST_for_clause) :
    (type == AST_if_clause)? pre_test_pass_func(node, AST_if_clause) :
    (type == AST_else_clause_mid)? pre_test_pass_func(node, AST_else_clause_mid) :
    (type == AST_else_clause_final)? pre_test_pass_func(node, AST_else_clause_final) :
    (type == AST_else_clause)? pre_test_pass_func(node, AST_else_clause) :
    (type == AST_switch_clause)? pre_test_pass_func(node, AST_switch_clause) :
    (type == AST_case_clause)? pre_test_pass_func(node, AST_case_clause) :
    (type == AST_default_clause)? pre_test_pass_func(node, AST_default_clause) :
    (type == AST_case_body)? pre_test_pass_func(node, AST_case_body) :
    (type == AST_try_clause)? pre_test_pass_func(node, AST_try_clause) :
    (type == AST_except_clause_mid)? pre_test_pass_func(node, AST_except_clause_mid) :
    (type == AST_except_clause_final)? pre_test_pass_func(node, AST_except_clause_final) :
    (type == AST_except_clause)? pre_test_pass_func(node, AST_except_clause) :
    (type == AST_break_statement)? pre_test_pass_func(node, AST_break_statement) :
    (type == AST_continue_statement)? pre_test_pass_func(node, AST_continue_statement) :
    (type == AST_trace_statement)? pre_test_pass_func(node, AST_trace_statement) :
    (type == AST_inline_statement)? pre_test_pass_func(node, AST_inline_statement) :
    (type == AST_yield_statement)? pre_test_pass_func(node, AST_yield_statement) :
    (type == AST_type_statement)? pre_test_pass_func(node, AST_type_statement) :
    (type == AST_exit_statement)? pre_test_pass_func(node, AST_exit_statement) :
    (type == AST_print_statement)? pre_test_pass_func(node, AST_print_statement) :
    (type == AST_return_statement)? pre_test_pass_func(node, AST_return_statement) :
    (type == AST_raise_statement)? pre_test_pass_func(node, AST_raise_statement) :
    (type == AST_start_function)? pre_test_pass_func(node, AST_start_function) :
    (type == AST_import_statement)? pre_test_pass_func(node, AST_import_statement) :
    (type == AST_operator)? pre_test_pass_func(node, AST_operator) :
    (type == AST_cast_statement)? pre_test_pass_func(node, AST_cast_statement) :
    (type == AST_case_item)? pre_test_pass_func(node, AST_case_item) :
    (type == AST_class_var_declaration)? pre_test_pass_func(node, AST_class_var_declaration) :

        dummy_function();

}

void post_test_pass(ast_node* node) {

    AstType type = ast_node_type(node);

    (type == AST_module)? post_test_pass_func(node, AST_module) :
    (type == AST_module_item)? post_test_pass_func(node, AST_module_item) :
    (type == AST_namespace_item)? post_test_pass_func(node, AST_namespace_item) :
    (type == AST_namespace_definition)? post_test_pass_func(node, AST_namespace_definition) :
    (type == AST_class_item)? post_test_pass_func(node, AST_class_item) :
    (type == AST_class_definition)? post_test_pass_func(node, AST_class_definition) :
    (type == AST_compound_name)? post_test_pass_func(node, AST_compound_name) :
    (type == AST_compound_ref_item)? post_test_pass_func(node, AST_compound_ref_item) :
    (type == AST_compound_reference)? post_test_pass_func(node, AST_compound_reference) :
    (type == AST_scope_operator)? post_test_pass_func(node, AST_scope_operator) :
    (type == AST_literal_type_name)? post_test_pass_func(node, AST_literal_type_name) :
    (type == AST_literal_value)? post_test_pass_func(node, AST_literal_value) :
    (type == AST_type_name)? post_test_pass_func(node, AST_type_name) :
    (type == AST_formatted_strg)? post_test_pass_func(node, AST_formatted_strg) :
    (type == AST_string_literal)? post_test_pass_func(node, AST_string_literal) :
    (type == AST_string_expr_item)? post_test_pass_func(node, AST_string_expr_item) :
    (type == AST_string_expr)? post_test_pass_func(node, AST_string_expr) :
    (type == AST_var_decl)? post_test_pass_func(node, AST_var_decl) :
    (type == AST_var_decl_list)? post_test_pass_func(node, AST_var_decl_list) :
    (type == AST_var_definition)? post_test_pass_func(node, AST_var_definition) :
    (type == AST_list_init)? post_test_pass_func(node, AST_list_init) :
    (type == AST_dict_init_element)? post_test_pass_func(node, AST_dict_init_element) :
    (type == AST_dict_init)? post_test_pass_func(node, AST_dict_init) :
    (type == AST_array_param)? post_test_pass_func(node, AST_array_param) :
    (type == AST_array_reference)? post_test_pass_func(node, AST_array_reference) :
    (type == AST_function_reference)? post_test_pass_func(node, AST_function_reference) :
    (type == AST_create_reference)? post_test_pass_func(node, AST_create_reference) :
    (type == AST_destroy_reference)? post_test_pass_func(node, AST_destroy_reference) :
    (type == AST_function_declaration)? post_test_pass_func(node, AST_function_declaration) :
    (type == AST_create_declaration)? post_test_pass_func(node, AST_create_declaration) :
    (type == AST_destroy_declaration)? post_test_pass_func(node, AST_destroy_declaration) :
    (type == AST_function_definition)? post_test_pass_func(node, AST_function_definition) :
    (type == AST_create_name)? post_test_pass_func(node, AST_create_name) :
    (type == AST_destroy_name)? post_test_pass_func(node, AST_destroy_name) :
    (type == AST_create_definition)? post_test_pass_func(node, AST_create_definition) :
    (type == AST_destroy_definition)? post_test_pass_func(node, AST_destroy_definition) :
    (type == AST_function_body)? post_test_pass_func(node, AST_function_body) :
    (type == AST_function_body_element)? post_test_pass_func(node, AST_function_body_element) :
    (type == AST_expression)? post_test_pass_func(node, AST_expression) :
    (type == AST_expr_primary)? post_test_pass_func(node, AST_expr_primary) :
    (type == AST_expression_list)? post_test_pass_func(node, AST_expression_list) :
    (type == AST_assignment_item)? post_test_pass_func(node, AST_assignment_item) :
    (type == AST_assignment)? post_test_pass_func(node, AST_assignment) :
    (type == AST_while_definition)? post_test_pass_func(node, AST_while_definition) :
    (type == AST_while_clause)? post_test_pass_func(node, AST_while_clause) :
    (type == AST_do_clause)? post_test_pass_func(node, AST_do_clause) :
    (type == AST_for_clause)? post_test_pass_func(node, AST_for_clause) :
    (type == AST_if_clause)? post_test_pass_func(node, AST_if_clause) :
    (type == AST_else_clause_mid)? post_test_pass_func(node, AST_else_clause_mid) :
    (type == AST_else_clause_final)? post_test_pass_func(node, AST_else_clause_final) :
    (type == AST_else_clause)? post_test_pass_func(node, AST_else_clause) :
    (type == AST_switch_clause)? post_test_pass_func(node, AST_switch_clause) :
    (type == AST_case_clause)? post_test_pass_func(node, AST_case_clause) :
    (type == AST_default_clause)? post_test_pass_func(node, AST_default_clause) :
    (type == AST_case_body)? post_test_pass_func(node, AST_case_body) :
    (type == AST_try_clause)? post_test_pass_func(node, AST_try_clause) :
    (type == AST_except_clause_mid)? post_test_pass_func(node, AST_except_clause_mid) :
    (type == AST_except_clause_final)? post_test_pass_func(node, AST_except_clause_final) :
    (type == AST_except_clause)? post_test_pass_func(node, AST_except_clause) :
    (type == AST_break_statement)? post_test_pass_func(node, AST_break_statement) :
    (type == AST_continue_statement)? post_test_pass_func(node, AST_continue_statement) :
    (type == AST_trace_statement)? post_test_pass_func(node, AST_trace_statement) :
    (type == AST_inline_statement)? post_test_pass_func(node, AST_inline_statement) :
    (type == AST_yield_statement)? post_test_pass_func(node, AST_yield_statement) :
    (type == AST_type_statement)? post_test_pass_func(node, AST_type_statement) :
    (type == AST_exit_statement)? post_test_pass_func(node, AST_exit_statement) :
    (type == AST_print_statement)? post_test_pass_func(node, AST_print_statement) :
    (type == AST_return_statement)? post_test_pass_func(node, AST_return_statement) :
    (type == AST_raise_statement)? post_test_pass_func(node, AST_raise_statement) :
    (type == AST_start_function)? post_test_pass_func(node, AST_start_function) :
    (type == AST_import_statement)? post_test_pass_func(node, AST_import_statement) :
    (type == AST_operator)? post_test_pass_func(node, AST_operator) :
    (type == AST_cast_statement)? post_test_pass_func(node, AST_cast_statement) :
    (type == AST_case_item)? post_test_pass_func(node, AST_case_item) :
    (type == AST_class_var_declaration)? pre_test_pass_func(node, AST_class_var_declaration) :

        dummy_function();

}

