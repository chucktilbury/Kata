/**
 * @file data.c
 *
 * @brief Implement the functions that travsrse the data AST nodes.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-11-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_ast.h"

/**
 *  type_name
*       = TOK_NUMBER
 *      / TOK_NOTHING
 *      / TOK_STRING
 *      / TOK_BOOLEAN
 *      / TOK_LIST
 *      / TOK_DICT
 *
 * @param node
 *
 */
int ast_type_name(AstTypeName* node) {

    assert(node != NULL);
    assert(node->node.type == AST_type_name);

    ENTER;
    RETV(0);
}

int ast_type_spec_element(AstTypeSpecElement* node) {

    assert(node != NULL);
    assert(node->node.type == AST_type_spec_element);

    ENTER;
    RETV(0);
}

int ast_type_spec(AstTypeSpec* node) {

    assert(node != NULL);
    assert(node->node.type == AST_type_spec);

    ENTER;
    RETV(0);
}

