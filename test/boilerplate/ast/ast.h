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
    AST_module = 3000,
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
    AST_func_decl,
    AST_func_definition,
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
} AstType;

typedef struct _ast_node_ {
    AstType type;
} ast_node;

#include "module.h"
#include "compound.h"
#include "literals.h"
#include "string.h"
#include "vars.h"
#include "list.h"
#include "func.h"
#include "func_body.h"
#include "expr.h"
#include "loop.h"
#include "flow.h"
#include "except.h"

#endif /* __AST_H__ */
