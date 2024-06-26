/**
 * @file ast.c
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#include "ast.h"
#include "errors.h"
#include "memory.h"
#include "trace.h"

void traverse_ast(ast_module* node, PassFunc pre, PassFunc post) {

    ENTER;
    if(0 == get_num_errors()) {
        if(node != NULL) {
            AST_CALLBACK(pre, node);
            traverse_module(node, pre, post);
            AST_CALLBACK(post, node);
        }
        else {
            TRACE("no ast present");
        }
    }
    else {
        TRACE("do not traverse with errors");
    }
    RET;
}

AstType ast_node_type(void* ptr) {

    return ((ast_node*)ptr)->type;
}

ast_node* create_ast_node(AstType type, size_t size) {

    ast_node* node = _ALLOC(size);
    node->type     = type;
    return node;
}

const char* nterm_to_str(ast_node* node) {

    AstType type = ast_node_type(node);

    return ((type == AST_module)                 ? "module" :
                    (type == AST_module_item)    ? "module_item" :
                    (type == AST_namespace_item) ? "namespace_item" :
                    (type == AST_namespace_definition) ?
                                                       "namespace_definition" :
                    (type == AST_class_item)         ? "class_item" :
                    (type == AST_class_definition)   ? "class_definition" :
                    (type == AST_compound_name)      ? "compound_name" :
                    (type == AST_compound_ref_item)  ? "compound_ref_item" :
                    (type == AST_compound_reference) ? "compound_reference" :
                    (type == AST_scope_operator)     ? "scope_operator" :
                    (type == AST_literal_type_name)  ? "literal_type_name" :
                    (type == AST_literal_value)      ? "literal_value" :
                    (type == AST_type_name)          ? "type_name" :
                    (type == AST_formatted_strg)     ? "formatted_strg" :
                    (type == AST_string_literal)     ? "string_literal" :
                    (type == AST_var_decl)           ? "var_decl" :
                    (type == AST_var_decl_list)      ? "var_decl_list" :
                    (type == AST_var_definition)     ? "var_definition" :
                    (type == AST_list_init)          ? "list_init" :
                    (type == AST_dict_init_element)  ? "dict_init_element" :
                    (type == AST_dict_init)          ? "dict_init" :
                    (type == AST_array_param)        ? "array_param" :
                    (type == AST_array_reference)    ? "array_reference" :
                    (type == AST_function_reference) ? "function_reference" :
                    (type == AST_create_reference)   ? "create_reference" :
                    (type == AST_destroy_reference)  ? "destroy_reference" :
                    (type == AST_function_declaration) ?
                                                        "function_declaration" :
                    (type == AST_create_declaration)  ? "create_declaration" :
                    (type == AST_destroy_declaration) ? "destroy_declaration" :
                    (type == AST_function_definition) ? "function_definition" :
                    (type == AST_create_name)         ? "create_name" :
                    (type == AST_destroy_name)        ? "destroy_name" :
                    (type == AST_create_definition)   ? "create_definition" :
                    (type == AST_destroy_definition)  ? "destroy_definition" :
                    (type == AST_function_body)       ? "function_body" :
                    (type == AST_function_body_element) ?
                                                        "function_body_element" :
                    (type == AST_expression)          ? "expression" :
                    (type == AST_expr_primary)        ? "expr_primary" :
                    (type == AST_expression_list)     ? "expression_list" :
                    (type == AST_assignment_item)     ? "assignment_item" :
                    (type == AST_assignment)          ? "assignment" :
                    (type == AST_while_definition)    ? "while_definition" :
                    (type == AST_while_clause)        ? "while_clause" :
                    (type == AST_do_clause)           ? "do_clause" :
                    (type == AST_for_clause)          ? "for_clause" :
                    (type == AST_if_clause)           ? "if_clause" :
                    (type == AST_else_clause)         ? "else_clause" :
                    (type == AST_elif_clause)         ? "elif_clause" :
                    (type == AST_final_clause)        ? "final_clause" :
                    (type == AST_try_clause)          ? "try_clause" :
                    (type == AST_except_clause)       ? "except_clause" :
                    (type == AST_break_statement)     ? "break_statement" :
                    (type == AST_continue_statement)  ? "continue_statement" :
                    (type == AST_inline_statement)    ? "inline_statement" :
                    (type == AST_yield_statement)     ? "yield_statement" :
                    (type == AST_return_statement)    ? "return_statement" :
                    (type == AST_raise_statement)     ? "raise_statement" :
                    (type == AST_trace_statement)     ? "trace_statement" :
                    (type == AST_print_statement)     ? "print_statement" :
                    (type == AST_exit_statement)      ? "exit_statement" :
                    (type == AST_start_function)      ? "start_function" :
                    (type == AST_operator)            ? "operator" :
                    (type == AST_cast_statement)      ? "cast_statement" :
                    (type == AST_import_statement)    ? "import_statement" :
                    (type == AST_type_name_list)      ? "type_name_list" :
                    (type == AST_function_assignment) ? "function_assignment" :
                    (type == AST_compound_name_list)  ? "compound_name_list" :
                    (type == AST_array_param_list)    ? "array_param_list" :
                    (type == AST_class_var_declaration) ?
                                                     "class_var_declaration" :
                                                     "UNKNOWN");
}
