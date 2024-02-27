/**
 * @file func.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#define USE_TRACE 1
#include "util.h"
#include "ast.h"

/**
 * @brief
 *
 *  function_reference
 *      = compound_name
 *              ( expression_list )+
 *              '(' ( compound_name ( ',' compound_name )* )+ ')'
 *
 * @param node
 *
 */
void traverse_function_reference(ast_function_reference* node) {

    assert(node != NULL);

    ENTER;
    RET;
}

/**
 * @brief
 *
 *  func_decl
 *      = 'function' SYMBOL '(' ( var_decl_list )* ')' '(' ( var_decl_list )* ')'
 *      / 'create' '(' ( var_decl_list )* ')'
 *      / 'destroy'
 *
 * @param node
 *
 */
void traverse_func_decl(ast_func_decl* node) {

    assert(node != NULL);

    ENTER;
    RET;
}

/**
 * @brief
 *
 *  func_definition
 *      = 'function' compound_name
 *              '(' ( var_decl_list )* ')' '(' ( var_decl_list )* ')' function_body
 *      / 'function' compound_name '.' 'create' '(' ( var_decl_list )* ')' function_body
 *      / 'function' compound_name '.' 'destroy' function_body
 *
 * @param node
 *
 */
void traverse_func_definition(ast_func_definition* node) {

    assert(node != NULL);

    ENTER;
    RET;
}

/**
 * @brief
 *
 *  function_body
 *      = '{' ( function_body_element )* '}'
 *
 * @param node
 *
 */
void traverse_function_body(ast_function_body* node) {

    assert(node != NULL);

    ENTER;
    RET;
}

/**
 * @brief
 *
 *  start_function
 *      = 'function' 'start' function_body
 *
 * @param node
 *
 */
void traverse_start_function(ast_start_function* node) {

    assert(node != NULL);

    ENTER;
    RET;
}
