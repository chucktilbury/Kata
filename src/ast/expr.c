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
#define USE_TRACE 1
#include "util.h"
#include "ast.h"

/**
 * @brief 
 * 
 * @param node 
 * 
 */
void traverse_expression(ast_expression* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    ast_node* nterm;

    if(node->list != NULL) {
        init_llist_iter(node->list);
        while(NULL != (nterm = iter_llist(node->list))) {
            if(AST_expr_primary == ast_node_type(nterm))
                traverse_expr_primary((ast_expr_primary*)nterm, func);
            else if(AST_operator == ast_node_type(nterm))
                traverse_operator((ast_operator*)nterm, func);
            else 
                RAISE(TRAVERSE_ERROR, "expected a primary or operator, but got %s", nterm_to_str(nterm));
        }
    }

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
 * @param func 
 * 
 */
void traverse_operator(ast_operator* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    TRACE_TERM(node->tok);
    RET;
}

/**
 * @brief 
 * 
 *  cast_statement
 *      = type_name ':' expression
 * 
 * @param node 
 * @param func 
 * 
 */
void traverse_cast_statement(ast_cast_statement* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    traverse_type_name(node->type, func);
    traverse_expression(node->expr, func);
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
void traverse_expr_primary(ast_expr_primary* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    ast_node* nterm = node->nterm;
    if(AST_literal_value == ast_node_type(nterm))
        traverse_literal_value((ast_literal_value*)nterm, func);
    else if(AST_compound_reference == ast_node_type(nterm))
        traverse_compound_reference((ast_compound_reference*)nterm, func);
    else 
        RAISE(TRAVERSE_ERROR, "expected an expr_primary, but got %s", nterm_to_str(nterm));

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
void traverse_expression_list(ast_expression_list* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    ast_node* nterm;

    init_llist_iter(node->list);
    while(NULL != (nterm = iter_llist(node->list)))
        traverse_expression((ast_expression*)nterm, func);
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
 * 
 * @param node 
 * 
 */
void traverse_assignment_item(ast_assignment_item* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    switch(ast_node_type(node->nterm)) {
        case AST_expression:
            traverse_expression((ast_expression*)node->nterm, func);
            break;
        case AST_list_init:
            traverse_list_init((ast_list_init*)node->nterm, func);
            break;
        case AST_dict_init:
            traverse_dict_init((ast_dict_init*)node->nterm, func);
            break;
        case AST_string_expr:
            traverse_string_expr((ast_string_expr*)node->nterm, func);
            break;
        case AST_cast_statement:
            traverse_cast_statement((ast_cast_statement*)node->nterm, func);
            break;
        default:
            RAISE(TRAVERSE_ERROR, "unexpected node type in %s: %d",
                    __func__, ast_node_type(node->nterm));
    }
    RET;
}

/**
 * @brief 
 * 
 *  assignment
 *      = compound_reference '=' assignment_item
 *      / compound_reference '+=' expression 
 *      / compound_reference '-=' expression
 *      / compound_reference '*=' expression
 *      / compound_reference '/=' expression
 *      / compound_reference '%=' expression
 * 
 * @param node 
 * 
 */
void traverse_assignment(ast_assignment* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    traverse_compound_reference(node->lhs, func);
    TRACE_TERM(node->oper);
    traverse_assignment_item((ast_assignment_item*)node->rhs, func);
    RET;
}


