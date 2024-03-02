/**
 * @file func.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __FUNC_H__
#define __FUNC_H__
#include "ast.h"

/**
 *  function_reference
 *      = compound_name
 *          ( expression_list )+
 *          '(' ( compound_name ( ',' compound_name )* )+ ')'
 */
typedef struct _ast_function_reference_ {
    ast_node node;
} ast_function_reference;

/**
 *  func_decl
 *      = ('virtual')? 'function' SYMBOL
 *          '(' ( var_decl_list )* ')'
 *          '(' ( var_decl_list )* ')'
 *      / ('virtual')? 'create' '(' ( var_decl_list )* ')'
 *      / ('virtual')? 'destroy'
 */
typedef struct _ast_func_decl_ {
    ast_node node;
} ast_func_decl;

/**
 *  func_definition
 *      = 'function' compound_name
 *          '(' ( var_decl_list )* ')'
 *          '(' ( var_decl_list )* ')' function_body
 *      / 'function' compound_name '.' 'create'
 *          '(' ( var_decl_list )* ')' function_body
 *      / 'function' compound_name '.' 'destroy' function_body
 */
typedef struct _ast_func_definition_ {
    ast_node node;
} ast_func_definition;

/**
 *  function_body
 *      = '{' ( function_body_element )* '}'
 */
typedef struct _ast_function_body_ {
    ast_node node;
} ast_function_body;

/**
 *  start_function
 *      = 'function' 'start' function_body
 */
typedef struct _ast_start_function_ {
    ast_node node;
} ast_start_function;

void traverse_function_reference(ast_function_reference* node, PassFunc func);
void traverse_func_decl(ast_func_decl* node, PassFunc func);
void traverse_func_definition(ast_func_definition* node, PassFunc func);
void traverse_function_body(ast_function_body* node, PassFunc func);
void traverse_start_function(ast_start_function* node, PassFunc func);

#endif /* __FUNC_H__ */
