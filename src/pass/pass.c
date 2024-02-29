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
#include "util.h"
#include "ast.h"
#include "trace.h"


/**
 * @brief Simple function that simply prints the node type and the number
 * to demonstrate that the function as actually called.
 * 
 * @param node 
 * @param number 
 * 
 */
void test_pass_func(ast_node* node, int number) {

    ENTER;
    TRACE("node type: %s(%d)", nterm_to_str(node), number);
    RET;
}

/**
 * @brief This function specifically does nothing. It is called when none of 
 * the nodes are a match in the test_pass().
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
 * all of the information needed by the pass must be provided in the caller.
 * 
 * @param node 
 * @param type 
 * @return * void 
 * 
 */
void test_pass(ast_node* node) {

    AstType type = ast_node_type(node);

    (type == AST_module)? test_pass_func(node, AST_module) :
    (type == AST_module_item)? test_pass_func(node, AST_module_item) :
    (type == AST_namespace_item)? test_pass_func(node, AST_namespace_item) :
    (type == AST_namespace_definition)? test_pass_func(node, AST_namespace_definition) :
    (type == AST_class_item)? test_pass_func(node, AST_class_item) :
    (type == AST_class_definition)? test_pass_func(node, AST_class_definition) :
    (type == AST_compound_name)? test_pass_func(node, AST_compound_name) :
    (type == AST_compound_ref_item)? test_pass_func(node, AST_compound_ref_item) :
    (type == AST_compound_reference)? test_pass_func(node, AST_compound_reference) :
    (type == AST_scope_operator)? test_pass_func(node, AST_scope_operator) :
    (type == AST_literal_type_name)? test_pass_func(node, AST_literal_type_name) :
    (type == AST_literal_value)? test_pass_func(node, AST_literal_value) :
    (type == AST_type_name)? test_pass_func(node, AST_type_name) :
    (type == AST_formatted_strg)? test_pass_func(node, AST_formatted_strg) :
    (type == AST_string_literal)? test_pass_func(node, AST_string_literal) :
    (type == AST_string_expr_item)? test_pass_func(node, AST_string_expr_item) :
    (type == AST_string_expr)? test_pass_func(node, AST_string_expr) :
    (type == AST_var_decl)? test_pass_func(node, AST_var_decl) :
    (type == AST_var_decl_list)? test_pass_func(node, AST_var_decl_list) :
    (type == AST_var_definition)? test_pass_func(node, AST_var_definition) :
    (type == AST_list_init)? test_pass_func(node, AST_list_init) :
    (type == AST_dict_init_element)? test_pass_func(node, AST_dict_init_element) :
    (type == AST_dict_init)? test_pass_func(node, AST_dict_init) :
    (type == AST_array_param)? test_pass_func(node, AST_array_param) :
    (type == AST_array_reference)? test_pass_func(node, AST_array_reference) :
    (type == AST_function_reference)? test_pass_func(node, AST_function_reference) :
    (type == AST_func_decl)? test_pass_func(node, AST_func_decl) :
    (type == AST_func_definition)? test_pass_func(node, AST_func_definition) :
    (type == AST_function_body)? test_pass_func(node, AST_function_body) :
    (type == AST_function_body_element)? test_pass_func(node, AST_function_body_element) :
    (type == AST_expression)? test_pass_func(node, AST_expression) :
    (type == AST_expr_primary)? test_pass_func(node, AST_expr_primary) :
    (type == AST_expression_list)? test_pass_func(node, AST_expression_list) :
    (type == AST_assignment_item)? test_pass_func(node, AST_assignment_item) :
    (type == AST_assignment)? test_pass_func(node, AST_assignment) :
    (type == AST_while_definition)? test_pass_func(node, AST_while_definition) :
    (type == AST_while_clause)? test_pass_func(node, AST_while_clause) :
    (type == AST_do_clause)? test_pass_func(node, AST_do_clause) :
    (type == AST_for_clause)? test_pass_func(node, AST_for_clause) :
    (type == AST_if_clause)? test_pass_func(node, AST_if_clause) :
    (type == AST_else_clause_mid)? test_pass_func(node, AST_else_clause_mid) :
    (type == AST_else_clause_final)? test_pass_func(node, AST_else_clause_final) :
    (type == AST_else_clause)? test_pass_func(node, AST_else_clause) :
    (type == AST_switch_clause)? test_pass_func(node, AST_switch_clause) :
    (type == AST_case_clause)? test_pass_func(node, AST_case_clause) :
    (type == AST_default_clause)? test_pass_func(node, AST_default_clause) :
    (type == AST_case_body)? test_pass_func(node, AST_case_body) :
    (type == AST_try_clause)? test_pass_func(node, AST_try_clause) :
    (type == AST_except_clause_mid)? test_pass_func(node, AST_except_clause_mid) :
    (type == AST_except_clause_final)? test_pass_func(node, AST_except_clause_final) :
    (type == AST_except_clause)? test_pass_func(node, AST_except_clause) :
    (type == AST_break_statement)? test_pass_func(node, AST_break_statement) :
    (type == AST_continue_statement)? test_pass_func(node, AST_continue_statement) :
    (type == AST_trace_statement)? test_pass_func(node, AST_trace_statement) :
    (type == AST_inline_statement)? test_pass_func(node, AST_inline_statement) :
    (type == AST_yield_statement)? test_pass_func(node, AST_yield_statement) :
    (type == AST_type_statement)? test_pass_func(node, AST_type_statement) :
    (type == AST_exit_statement)? test_pass_func(node, AST_exit_statement) :
    (type == AST_print_statement)? test_pass_func(node, AST_print_statement) :
    (type == AST_return_statement)? test_pass_func(node, AST_return_statement) :
    (type == AST_raise_statement)? test_pass_func(node, AST_raise_statement) :
    (type == AST_start_function)? test_pass_func(node, AST_start_function) :
    (type == AST_import_statement)? test_pass_func(node, AST_import_statement) : 
        dummy_function();

}
