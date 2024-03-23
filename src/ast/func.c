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
 *      = compound_reference expression_list compound_name_list
 *
 * @param node
 * @param func
 *
 */
void traverse_function_reference(ast_function_reference* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_compound_reference(node->name, func);
    traverse_expression_list(node->inp, func);
    traverse_compound_name_list(node->outp, func);
    RET;
}

/**
 *
 *  create_reference
 *      = create_name expression_list
 */
void traverse_create_reference(ast_create_reference* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_create_name(node->name, func);
    traverse_expression_list(node->inp, func);
    RET;
}

/**
 *
 *  destroy_reference
 *      = destroy_name
 */
void traverse_destroy_reference(ast_destroy_reference* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_destroy_name(node->name, func);
    RET;
}

/**
 * @brief Note that this rule only specifies whether the function is virtual
 * or not. The 'funciton' keyword is optional and ignored. (for documentation
 * and/or clarity) The two keywords are optional and can appear in any order.
 *
 *  func_qualifier
 *      = ('virtual')?
 *      / ('function')?
 *
 * @param node
 * @param func
 *
 */
void traverse_func_qualifier(ast_func_qualifier* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    TRACE("is_virtual: %s", (node->is_virtual)? "true": "false");
    TRACE("func_seen: %s", (node->func_seen)? "true": "false");
    RET;
}

/**
 * @brief
 *
 *  function_declaration
 *      = (func_qualifier)? SYMBOL type_name_list type_name_list
 *
 * @param node
 * @param func
 *
 */
void traverse_function_declaration(ast_function_declaration* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_func_qualifier(node->qual, func);
    TRACE_TERM(node->name);
    traverse_type_name_list(node->inputs, func);
    traverse_type_name_list(node->outputs, func);
    RET;
}

/**
 * @brief
 *
 *  create_declaration
 *      = (func_qualifier)? 'create' type_name_list
 *
 * @param node
 * @param func
 *
 */
void traverse_create_declaration(ast_create_declaration* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_func_qualifier(node->qual, func);
    traverse_type_name_list(node->inputs, func);
    RET;
}

/**
 * @brief
 *
 *  destroy_declaration
 *      = (func_qualifier)? 'destroy'
 *
 * @param node
 * @param func
 *
 */
void traverse_destroy_declaration(ast_destroy_declaration* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_func_qualifier(node->qual, func);
    RET;
}

/**
 * @brief
 *
 *  function_definition
 *      = (func_qualifier)? compound_name
 *          var_decl_list var_decl_list function_body
 *
 * @param node
 * @param func
 *
 */
void traverse_function_definition(ast_function_definition* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_func_qualifier(node->qual, func);
    traverse_compound_reference(node->name, func);
    traverse_var_decl_list(node->inputs, func);
    traverse_var_decl_list(node->outputs, func);
    traverse_function_body(node->body, func);
    RET;
}

/**
 * @brief
 *
 *  ctor_name
 *      = SYMBOL ('.' SYMBOL)? '.' 'create'
 *
 * @param node
 * @param func
 *
 */
void traverse_create_name(ast_create_name* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    Token* tok;
    init_llist_iter(node->list);
    while(NULL != (tok = iter_llit(node->list)))
        TRACE_TERM(tok);
    RET;
}

/**
 * @brief
 *
 *  dtor_name
 *      = SYMBOL ('.' SYMBOL)? '.' 'destroy'
 *
 * @param node
 * @param func
 *
 */
void traverse_destroy_name(ast_destroy_name* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    Token* tok;
    init_llist_iter(node->list);
    while(NULL != (tok = iter_llit(node->list)))
        TRACE_TERM(tok);
    RET;
}

/**
 * @brief
 *
 *  create_definition
 *      = (func_qualifier)? ctor_name var_decl_list function_body
 *
 * @param node
 * @param func
 *
 */
void traverse_create_definition(ast_create_definition* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_create_name(node->name, func);
    traverse_func_qualifier(node->qual, func);
    traverse_var_decl_list(node->inputs, func);
    traverse_function_body(node->body, func);
    RET;
}

/**
 * @brief
 *
 *  destroy_definition
 *      = (func_qualifier)? dtor_name function_body
 *
 * @param node
 * @param func
 *
 */
void traverse_destroy_definition(ast_destroy_definition* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_destroy_name(node->name, func);
    traverse_func_qualifier(node->qual, func);
    traverse_function_body(node->body, func);
    RET;
}

/**
 * @brief
 *
 *  function_body
 *      = '{' ( function_body_element )* '}'
 *
 * @param node
 * @param func
 *
 */
void traverse_function_body(ast_function_body* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    ast_function_body_element* elem;
    init_llist(node->list);
    while(NULL != (elem = iter_llist(node->list)))
        traverse_function_body_element(elem, func);
    RET;
}

/**
 * @brief
 *
 *  start_function
 *      = ('function')? 'start' function_body
 *
 * @param node
 * @param func
 *
 */
void traverse_start_function(ast_start_function* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_function_body(node->body, func);
    RET;
}

/**
 *  function_assignment
 *      = compound_reference type_name_list type_name_list
 */
void traverse_function_assignment(ast_function_assignment* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    AST_CALLBACK(func, node);
    traverse_compound_reference(node->name, func);
    traverse_type_name_list(node->inp, func);
    traverse_type_name_list(node->outp, func);
    RET;
}

