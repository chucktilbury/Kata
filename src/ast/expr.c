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
    RET;
}

/**
 * @brief 
 * 
 *  expr_primary
 *      = literal_value
 *      / compound_reference
 *      / ( type_name )? '(' expression ')'
 * 
 * @param node 
 * 
 */
void traverse_expr_primary(ast_expr_primary* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
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
 * 
 * @param node 
 * 
 */
void traverse_assignment_item(ast_assignment_item* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
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
    RET;
}


