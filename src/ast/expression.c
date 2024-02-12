/**
 * @file expression.c
 *
 * @brief Implement the functions that traverse the expressions AST nodes.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-11-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_ast.h"


int ast_primary_expression(AstPrimaryExpression* node) {

    assert(node != NULL);
    assert(node->node.type == AST_primary_expression);

    ENTER;
    RETV(0);
}

int ast_expression_list(AstExpressionList* node) {

    assert(node != NULL);
    assert(node->node.type == AST_expression_list);

    ENTER;
    RETV(0);
}

int ast_expression(AstExpression* node) {

    assert(node != NULL);
    assert(node->node.type == AST_expression);

    ENTER;
    RETV(0);
}

int ast_cast_expression(AstCastExpression* node) {

    assert(node != NULL);
    assert(node->node.type == AST_cast_expression);

    ENTER;
    RETV(0);
}

