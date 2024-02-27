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

    (type == AST_module)? test_pass_func(node, 1) :
    (type == AST_module_item)? test_pass_func(node, 1) :
    (type == AST_namespace_item)? test_pass_func(node, 1) :
    (type == AST_namespace_definition)? test_pass_func(node, 1) :
    (type == AST_class_item)? test_pass_func(node, 1) :
    (type == AST_class_definition)? test_pass_func(node, 1) :
    (type == AST_compound_name)? test_pass_func(node, 1) :
    (type == AST_compound_ref_item)? test_pass_func(node, 1) :
    (type == AST_compound_reference)? test_pass_func(node, 1) :
    (type == AST_scope_operator)? test_pass_func(node, 1) :
    (type == AST_literal_type_name)? test_pass_func(node, 1) :
    (type == AST_literal_value)? test_pass_func(node, 1) :
    (type == AST_type_name)? test_pass_func(node, 1) :
    (type == AST_formatted_strg)? test_pass_func(node, 1) :
    (type == AST_string_literal)? test_pass_func(node, 1) :
    (type == AST_string_expr_item)? test_pass_func(node, 1) :
    (type == AST_string_expr)? test_pass_func(node, 1) :
    (type == AST_var_decl)? test_pass_func(node, 1) :
    (type == AST_var_decl_list)? test_pass_func(node, 1) :
    (type == AST_var_definition)? test_pass_func(node, 1) :
    (type == AST_list_init)? test_pass_func(node, 1) :
    (type == AST_dict_init_element)? test_pass_func(node, 1) :
    (type == AST_dict_init)? test_pass_func(node, 1) :
    (type == AST_array_param)? test_pass_func(node, 1) :
    (type == AST_array_reference)? test_pass_func(node, 1) :
    (type == AST_function_reference)? test_pass_func(node, 1) :
    (type == AST_func_decl)? test_pass_func(node, 1) :
    (type == AST_func_definition)? test_pass_func(node, 1) :
    (type == AST_function_body)? test_pass_func(node, 1) :
    (type == AST_function_body_element)? test_pass_func(node, 1) :

    (type == AST_expression)? test_pass_func(node, 2) :
    (type == AST_expr_primary)? test_pass_func(node, 2) :
    (type == AST_expression_list)? test_pass_func(node, 2) :
    (type == AST_assignment_item)? test_pass_func(node, 2) :
    (type == AST_assignment)? test_pass_func(node, 2) :
    (type == AST_while_definition)? test_pass_func(node, 2) :
    (type == AST_while_clause)? test_pass_func(node, 2) :
    (type == AST_do_clause)? test_pass_func(node, 2) :
    (type == AST_for_clause)? test_pass_func(node, 2) :
    (type == AST_if_clause)? test_pass_func(node, 2) :
    (type == AST_else_clause_mid)? test_pass_func(node, 2) :
    (type == AST_else_clause_final)? test_pass_func(node, 2) :
    (type == AST_else_clause)? test_pass_func(node, 2) :
    (type == AST_switch_clause)? test_pass_func(node, 2) :
    (type == AST_case_clause)? test_pass_func(node, 2) :
    (type == AST_default_clause)? test_pass_func(node, 2) :
    (type == AST_case_body)? test_pass_func(node, 2) :
    (type == AST_try_clause)? test_pass_func(node, 2) :
    (type == AST_except_clause_mid)? test_pass_func(node, 2) :
    (type == AST_except_clause_final)? test_pass_func(node, 2) :
    (type == AST_except_clause)? test_pass_func(node, 2) :
    (type == AST_break_statement)? test_pass_func(node, 2) :
    (type == AST_continue_statement)? test_pass_func(node, 2) :
    (type == AST_trace_statement)? test_pass_func(node, 2) :
    
    (type == AST_inline_statement)? test_pass_func(node, 3) :
    (type == AST_yield_statement)? test_pass_func(node, 3) :
    (type == AST_type_statement)? test_pass_func(node, 3) :
    (type == AST_exit_statement)? test_pass_func(node, 3) :
    (type == AST_print_statement)? test_pass_func(node, 3) :
    (type == AST_return_statement)? test_pass_func(node, 3) :
    (type == AST_raise_statement)? test_pass_func(node, 3) :
    (type == AST_start_function)? test_pass_func(node, 3) :
    (type == AST_import_statement)? test_pass_func(node, 3) : 
        dummy_function();

}
