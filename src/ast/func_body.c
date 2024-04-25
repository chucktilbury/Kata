/**
 * @file func_body.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#include <assert.h>

#include "trace.h"
#include "ast.h"
#include "errors.h"

/**
 * @brief
 *
 * function_body_element
 *      = var_definition
 *      / function_reference
 *      / create_reference
 *      / destroy_reference
 *      / assignment
 *      / while_clause
 *      / do_clause
 *      / for_clause
 *      / if_clause
 *      / try_clause
 *      / switch_clause
 *      / break_statement
 *      / continue_statement
 *      / trace_statement
 *      / inline_statement
 *      / yield_statement
 *      / type_statement
 *      / return_statement
 *      / raise_statement
 *      / function_body
 *
 * @param node
 *
 */
void traverse_function_body_element(ast_function_body_element* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    switch(ast_node_type(node->nterm)) {
        case AST_var_definition:
            traverse_var_definition((ast_var_definition*)node->nterm, pre, post);
            break;
        case AST_function_reference:
            traverse_function_reference((ast_function_reference*)node->nterm, pre, post);
            break;
        case AST_create_reference:
            traverse_create_reference((ast_create_reference*)node->nterm, pre, post);
            break;
        case AST_destroy_reference:
            traverse_destroy_reference((ast_destroy_reference*)node->nterm, pre, post);
            break;
        case AST_assignment:
            traverse_assignment((ast_assignment*)node->nterm, pre, post);
            break;
        case AST_while_clause:
            traverse_while_clause((ast_while_clause*)node->nterm, pre, post);
            break;
        case AST_do_clause:
            traverse_do_clause((ast_do_clause*)node->nterm, pre, post);
            break;
        case AST_for_clause:
            traverse_for_clause((ast_for_clause*)node->nterm, pre, post);
            break;
        case AST_if_clause:
            traverse_if_clause((ast_if_clause*)node->nterm, pre, post);
            break;
        case AST_try_clause:
            traverse_try_clause((ast_try_clause*)node->nterm, pre, post);
            break;
        case AST_switch_clause:
            traverse_switch_clause((ast_switch_clause*)node->nterm, pre, post);
            break;
        case AST_break_statement:
            traverse_break_statement((ast_break_statement*)node->nterm, pre, post);
            break;
        case AST_continue_statement:
            traverse_continue_statement((ast_continue_statement*)node->nterm, pre, post);
            break;
        case AST_inline_statement:
            traverse_inline_statement((ast_inline_statement*)node->nterm, pre, post);
            break;
        case AST_yield_statement:
            traverse_yield_statement((ast_yield_statement*)node->nterm, pre, post);
            break;
        case AST_type_statement:
            traverse_type_statement((ast_type_statement*)node->nterm, pre, post);
            break;
        case AST_return_statement:
            traverse_return_statement((ast_return_statement*)node->nterm, pre, post);
            break;
        case AST_raise_statement:
            traverse_raise_statement((ast_raise_statement*)node->nterm, pre, post);
            break;
        case AST_function_body:
            traverse_function_body((ast_function_body*)node->nterm, pre, post);
            break;
        default:
            fatal_error("unknown node type in %s: %s", __func__, nterm_to_str(node->nterm));
            break;
    }

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  break_statement
 *      = 'break'
 *
 * @param node
 *
 */
void traverse_break_statement(ast_break_statement* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  continue_statement
 *      = 'continue'
 *
 * @param node
 *
 */
void traverse_continue_statement(ast_continue_statement* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  inline_statement
 *      = 'inline' '{' RAW_TEXT '}'
 *
 * @param node
 *
 */
void traverse_inline_statement(ast_inline_statement* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->tok);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  yield_statement
 *      = 'yield' '(' compound_reference ')'
 *
 * @param node
 *
 */
void traverse_yield_statement(ast_yield_statement* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_compound_reference(node->ref, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  type_statement
 *      = 'type' '(' compound_reference ')'
 *
 * @param node
 *
 */
void traverse_type_statement(ast_type_statement* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_compound_reference(node->ref, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  return_statement
 *      = 'return' ( '(' ( expression )? ')' )?
 *
 * @param node
 *
 */
void traverse_return_statement(ast_return_statement* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_expression(node->expr, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  raise_statement
 *      = 'raise' '(' SYMBOL ',' formatted_strg ')'
 *
 * @param node
 *
 */
void traverse_raise_statement(ast_raise_statement* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    TRACE_TERM(node->symb);
    traverse_formatted_strg(node->str, pre, post);

    AST_CALLBACK(post, node);
    RET;
}


