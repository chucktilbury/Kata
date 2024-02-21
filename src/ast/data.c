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
void ast_type_name(AstTypeName* node) {

    assert(node != NULL);
    assert(node->node.type == AST_type_name);

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));
    if(AST_type_name == get_ast_node_type((AstNode*)node)) {
        TRACE_TERM(node->tok);
    }
    else {
        RAISE(AST_TRAVERSE_ERROR, "expected a type name but got %s", n_to_str((AstNode*)node));
    }

    RET;
}

/**
 * type_spec_element
 *      = type_name
 *      / compound_name
 *
 * @param node
 *
 */
void ast_type_spec_element(AstTypeSpecElement* node) {

    assert(node != NULL);
    assert(node->node.type == AST_type_spec_element);

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));
    switch(get_ast_node_type((AstNode*)node->elem)) {
        case AST_type_name:
            ast_type_name((AstTypeName*)node->elem);
            break;
        case AST_compound_name:
            ast_compound_name((AstCompoundName*)node->elem);
            break;
        default:
            RAISE(AST_TRAVERSE_ERROR, "expected a type spec element but got %s", n_to_str((AstNode*)node->elem));
    }

    RET;
}

/**
 * type_spec
 *      = (CONST)? type_spec_element
 *
 * @param node
 *
 */
void ast_type_spec(AstTypeSpec* node) {

    assert(node != NULL);
    assert(node->node.type == AST_type_spec);

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));
    TRACE("CONST: %s", node->is_const? "true": "false");
    ast_type_spec_element(node->elem);

    RET;
}

