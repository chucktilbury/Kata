/**
 * @file func.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-26-2024
 * @copyright Copyright (c) 2024
 */
#define USE_TRACE 1
#include "util.h"
#include "parse.h"
#include "scanner.h"

/**
 * @brief
 *
 *  function_reference
 *      = compound_reference expression_list compound_name_list
 *
 * @return ast_function_reference*
 *
 */
ast_function_reference* parse_function_reference() {

    ENTER;
    ast_function_reference* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  create_reference
 *      = create_name expression_list
 *
 * @return ast_create_reference*
 *
 */
ast_create_reference* parse_create_reference() {

    ENTER;
    ast_create_reference* node = NULL;

    RETV(node);
}


/**
 * @brief
 *
 *  destroy_reference
 *      = destroy_name
 *
 * @return ast_destroy_reference*
 *
 */
ast_destroy_reference* parse_destroy_reference() {

    ENTER;
    ast_destroy_reference* node = NULL;

    RETV(node);
}

/**
 * @brief The function qualifier has one,the other or both, but only one of
 * each.
 *
 *  func_qualifier
 *      = ('virtual')?
 *      / ('function')?
 *
 * @return ast_func_qualifier*
 *
 */
ast_func_qualifier* parse_func_qualifier() {

    ENTER;
    ast_func_qualifier* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  function_declaration
 *      = (func_qualifier)? SYMBOL type_name_list type_name_list
 *
 * @return ast_function_declaration*
 *
 */
ast_function_declaration* parse_function_declaration() {

    ENTER;
    ast_function_declaration* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  create_declaration
 *      = (func_qualifier)? 'create' type_name_list
 *
 * @return ast_create_declaration*
 *
 */
ast_create_declaration* parse_create_declaration() {

    ENTER;
    ast_create_declaration* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  destroy_declaration
 *      = (func_qualifier)? 'destroy'
 *
 * @return ast_destroy_declaration*
 *
 */
ast_destroy_declaration* parse_destroy_declaration() {

    ENTER;
    ast_destroy_declaration* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  function_definition
 *      = (func_qualifier)? compound_name
 *          var_decl_list var_decl_list function_body
 *
 * @return ast_function_definition*
 *
 */
ast_function_definition* parse_function_definition() {

    ENTER;
    ast_function_definition* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  ctor_name
 *      = SYMBOL ('.' SYMBOL)? '.' 'create'
 *
 * @return ast_ctor_name*
 *
 */
ast_ctor_name* parse_ctor_name() {

    ENTER;
    ast_ctor_name* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  dtor_name
 *      = SYMBOL ('.' SYMBOL)? '.' 'destroy'
 *
 * @return ast_dtor_name*
 *
 */
ast_dtor_name* parse_dtor_name() {

    ENTER;
    ast_dtor_name* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  create_definition
 *      = (func_qualifier)? ctor_name var_decl_list function_body
 *
 * @return ast_create_definition*
 *
 */
ast_create_definition* parse_create_definition() {

    ENTER;
    ast_create_definition* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  destroy_definition
 *      = (func_qualifier)? dtor_name function_body
 *
 * @return ast_destroy_definition*
 *
 */
ast_destroy_definition* parse_destroy_definition() {

    ENTER;
    ast_destroy_definition* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  function_body
 *      = '{' ( function_body_element )* '}'
 *
 * @return ast_function_body*
 *
 */
ast_function_body* parse_function_body() {

    ENTER;
    ast_function_body* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  start_function
 *      = ('function')? 'start' function_body
 *
 * @return ast_start_function*
 *
 */
ast_start_function* parse_start_function() {

    ENTER;
    ast_start_function* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  function_assignment
 *      = compound_reference type_name_list type_name_list
 *
 * @return ast_function_assignment*
 *
 */
ast_function_assignment* parse_function_assignment() {


    ENTER;
    ast_function_assignment* node = NULL;

    RETV(node);
}