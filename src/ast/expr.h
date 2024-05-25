/**
 * @file expr.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __EXPR_H__
#define __EXPR_H__
#include "ast.h"

/**
 *  Note that this does not match what is actually implemented. Expressions
 *  are actually implemented using the shunting yard algoritim. This is here
 *  as a place marker and illustration.
 *
 *  expression
 *      = expr_and 'or' expr_or
 *      / expr_equ 'and' expr_and
 *      / expr_mag '==' expr_equ
 *      / expr_mag '!=' expr_equ
 *      / expr_term '<' expr_mag
 *      / expr_term '>' expr_mag
 *      / expr_term '<=' expr_mag
 *      / expr_term '>=' expr_mag
 *      / expr_fact '+' expr_term
 *      / expr_fact '-' expr_term
 *      / expr_pow '*' expr_fact
 *      / expr_pow '/' expr_fact
 *      / expr_pow '%' expr_fact
 *      / expr_unary '^' expr_pow
 *      / '!' expr_primary
 *      / '-' expr_primary
 *      / '(' expression ')'
 */
typedef struct _ast_expression_ {
    ast_node node;
    LinkList* list;
    int expr_type;    // the type of the expression result
    int operand_type; // the type of operand that is being operated upon
} ast_expression;

/**
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
 */
typedef struct _ast_operator_ {
    ast_node node;
    Token* tok;
} ast_operator;

/**
 * cast_statement
 *      = type_name '(' expression ')'
 */
typedef struct _ast_cast_statement_ {
    ast_node node;
    struct _ast_type_name_* type;
    struct _ast_expression_* expr;
} ast_cast_statement;

/**
 *  expr_primary
 *      = literal_value
 *      / compound_reference
 *      / cast_statement
 */
typedef struct _ast_expr_primary_ {
    ast_node node;
    ast_node* nterm;
} ast_expr_primary;

/**
 *  expression_list
 *      = '(' (expression ( ',' expression )*)? ')'
 */
typedef struct _ast_expression_list_ {
    ast_node node;
    LinkList* list;
} ast_expression_list;

/**
 *  assignment_item
 *      = expression
 *      / list_init
 *      / dict_init
 *      / function_assignment
 */
typedef struct _ast_assignment_item_ {
    ast_node node;
    ast_node* nterm;
} ast_assignment_item;

/**
 *  NOTE: support the += for arrays
 *
 *  assignment
 *      = compound_reference '=' ( assignment_item / compound_reference )
 *      / compound_reference '+=' expression
 *      / compound_reference '-=' expression
 *      / compound_reference '*=' expression
 *      / compound_reference '/=' expression
 *      / compound_reference '%=' expression
 */
typedef struct _ast_assignment_ {
    ast_node node;
    struct _ast_compound_reference_* lhs;
    ast_node* rhs;
    Token* oper;
} ast_assignment;

void traverse_expression(ast_expression* node, PassFunc pre, PassFunc post);
void traverse_expr_primary(ast_expr_primary* node, PassFunc pre, PassFunc post);
void traverse_expression_list(ast_expression_list* node, PassFunc pre, PassFunc post);
void traverse_assignment_item(ast_assignment_item* node, PassFunc pre, PassFunc post);
void traverse_assignment(ast_assignment* node, PassFunc pre, PassFunc post);
void traverse_operator(ast_operator* node, PassFunc pre, PassFunc post);


#endif /* __EXPR_H__ */
