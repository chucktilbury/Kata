/**
 * @file expr.c
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
 *      
 * @return ast_expression* 
 * 
 */
ast_expression* parse_expression(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_expression* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  expr_primary
 *      = literal_value
 *      / compound_reference
 *      / ( type_name )? '(' expression ')'
 *      
 * @return ast_expr_primary* 
 * 
 */
ast_expr_primary* parse_expr_primary(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_expr_primary* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  expression_list
 *      = '(' (expression ( ',' expression )*)? ')'
 *      
 * @return ast_expression_list* 
 * 
 */
ast_expression_list* parse_expression_list(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_expression_list* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  assignment_item
 *      = expression
 *      / list_init
 *      / dict_init
 *      / string_expr
 *      
 * @return ast_assignment_item* 
 * 
 */
ast_assignment_item* parse_assignment_item(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_assignment_item* node = NULL;

    RETV(node);
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
 * @return ast_assignment* 
 * 
 */
ast_assignment* parse_assignment(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_assignment* node = NULL;

    RETV(node);
}


