/**
 * @file ast.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __AST_H__
#define __AST_H__

typedef enum {
    AST_FIRST = 3000,
    AST_module = AST_FIRST,
    AST_module_item,
    AST_namespace_item,
    AST_namespace_definition,
    AST_class_item,
    AST_class_definition,
    AST_compound_name,
    AST_compound_ref_item,
    AST_compound_reference,
    AST_scope_operator,
    AST_literal_type_name,
    AST_literal_value,
    AST_type_name,
    AST_formatted_strg,
    AST_string_literal,
    AST_string_expr_item,
    AST_string_expr,
    AST_var_decl,
    AST_var_decl_list,
    AST_var_definition,
    AST_list_init,
    AST_dict_init_element,
    AST_dict_init,
    AST_array_param,
    AST_array_reference,
    AST_function_reference,
    AST_create_reference,
    AST_destroy_reference,
    AST_func_qualifier,
    AST_function_declaration,
    AST_create_declaration,
    AST_destroy_declaration,
    AST_function_definition,
    AST_create_name,
    AST_destroy_name,
    AST_create_definition,
    AST_destroy_definition,
    AST_function_body,
    AST_function_body_element,
    AST_expression,
    AST_expr_primary,
    AST_expression_list,
    AST_assignment_item,
    AST_assignment,
    AST_while_definition,
    AST_while_clause,
    AST_do_clause,
    AST_for_clause,
    AST_if_clause,
    AST_else_clause_mid,
    AST_else_clause_final,
    AST_else_clause,
    AST_switch_clause,
    AST_case_clause,
    AST_default_clause,
    AST_case_body,
    AST_try_clause,
    AST_except_clause_mid,
    AST_except_clause_final,
    AST_except_clause,
    AST_break_statement,
    AST_continue_statement,
    AST_trace_statement,
    AST_inline_statement,
    AST_yield_statement,
    AST_type_statement,
    AST_exit_statement,
    AST_print_statement,
    AST_return_statement,
    AST_raise_statement,
    AST_start_function,
    AST_import_statement,
    AST_operator,
    AST_cast_statement,
    AST_case_item,
    AST_type_name_list,
    AST_function_assignment,
    AST_compound_name_list,
    AST_LAST,   // number of non-terminals in the list
} AstType;

typedef struct _ast_node_ {
    AstType type;
    bool seen; // used to detect infinite recursion.
} ast_node;

#include "scanner.h"
#include "trace.h"
#include "pass.h"
#include "parse_state.h"

#include "module.h"
#include "compound.h"
#include "literals.h"
#include "strg.h"
#include "vars.h"
#include "list.h"
#include "func.h"
#include "func_body.h"
#include "expr.h"
#include "loop.h"
#include "flow.h"
#include "except.h"
#include "import.h"

// public interface
void traverse_ast(ast_module* node, PassFunc pre, PassFunc post);
AstType ast_node_type(void* node);
const char* nterm_to_str(ast_node* node);
ast_node* create_ast_node(AstType type, size_t size);

// hide the icky syntax
#define AST_CALLBACK(f, n) do { if(NULL != (f)) (*(f))((ast_node*)(n)); } while(false)
#define CREATE_AST_NODE(t, n) (n*)create_ast_node((t), sizeof(n))
#define TRAVERSE_ERROR AST_FIRST

#endif /* __AST_H__ */

