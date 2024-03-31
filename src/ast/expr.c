/**
 * @file expr.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#include <assert.h>

#include "link_list.h"
#include "trace.h"
#include "ast.h"
#include "errors.h"

/**
 * @brief
 *
 * @param node
 *
 */
void traverse_expression(ast_expression* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);
    ast_node* nterm;

    if(node->list != NULL) {
        void* mark = NULL;
        while(NULL != (nterm = iter_link_list(node->list, &mark))) {
            if(AST_expr_primary == ast_node_type(nterm))
                traverse_expr_primary((ast_expr_primary*)nterm, pre, post);
            else if(AST_operator == ast_node_type(nterm))
                traverse_operator((ast_operator*)nterm, pre, post);
            else
                fatal_error("expected a primary or operator, but got %s", nterm_to_str(nterm));
        }
    }

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  ast_operator
 *      = AND
 *      / OR
 *      / '=='
 *      / '!='
 *      / '<'
 *      / '>'
 *      / '<='
 *      / '>='
 *      / '+'
 *      / '-'
 *      / '*'
 *      / '/'
 *      / '%'
 *      / '^'
 *      / UNARY_MINUS
 *      / '!'
 *      / '('
 *      / ')'

 * @param node
 * @param pre
 *
 */
void traverse_operator(ast_operator* node, PassFunc pre, PassFunc post) {

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
 *  cast_statement
 *      = type_name ':' expression
 *
 * @param node
 * @param pre
 *
 */
void traverse_cast_statement(ast_cast_statement* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_type_name(node->type, pre, post);
    traverse_expression(node->expr, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  expr_primary
 *      = literal_value
 *      / compound_reference
 *
 * @param node
 *
 */
void traverse_expr_primary(ast_expr_primary* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    ast_node* nterm = node->nterm;
    if(AST_literal_value == ast_node_type(nterm))
        traverse_literal_value((ast_literal_value*)nterm, pre, post);
    else if(AST_compound_reference == ast_node_type(nterm))
        traverse_compound_reference((ast_compound_reference*)nterm, pre, post);
    else
        fatal_error("expected an expr_primary, but got %s", nterm_to_str(nterm));

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  expression_list
 *      = '(' (expression ( ',' expression )*)? ')'
 *
 * @param node
 *
 */
void traverse_expression_list(ast_expression_list* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    ast_node* nterm;

    void* mark = NULL;
    while(NULL != (nterm = iter_link_list(node->list, &mark)))
        traverse_expression((ast_expression*)nterm, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  assignment_item
 *      = expression
 *      / list_init
 *      / dict_init
 *      / string_expr
 *      / cast_statement
 *      / function_assignment
 *
 * @param node
 *
 */
void traverse_assignment_item(ast_assignment_item* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    switch(ast_node_type(node->nterm)) {
        case AST_expression:
            traverse_expression((ast_expression*)node->nterm, pre, post);
            break;
        case AST_list_init:
            traverse_list_init((ast_list_init*)node->nterm, pre, post);
            break;
        case AST_dict_init:
            traverse_dict_init((ast_dict_init*)node->nterm, pre, post);
            break;
        case AST_string_expr:
            traverse_string_expr((ast_string_expr*)node->nterm, pre, post);
            break;
        case AST_cast_statement:
            traverse_cast_statement((ast_cast_statement*)node->nterm, pre, post);
            break;
        case AST_function_assignment:
            traverse_function_assignment((ast_function_assignment*)node->nterm, pre, post);
            break;
        default:
            fatal_error("unexpected node type in %s: %d",
                    __func__, ast_node_type(node->nterm));
    }

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  assignment
 *      = compound_reference '=' assignment_item
 *      / compound_reference '+=' assignment_item
 *      / compound_reference '-=' expression
 *      / compound_reference '*=' expression
 *      / compound_reference '/=' expression
 *      / compound_reference '%=' expression
 *
 * @param node
 *
 */
void traverse_assignment(ast_assignment* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    traverse_compound_reference(node->lhs, pre, post);
    TRACE_TERM(node->oper);
    traverse_assignment_item((ast_assignment_item*)node->rhs, pre, post);

    AST_CALLBACK(post, node);
    RET;
}


