/**
 * @file namespace.c
 *
 * @brief Namespace AST functions
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-11-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_ast.h"

/**
 *  namespace_element
 *      = namespace_definition
 *      | type_spec
 *      | scope_operator
 *
 * @param node
 *
 */
int ast_namespace_element(AstNamespaceElement* node) {

    assert(node != NULL);
    assert(node->node.type == AST_namespace_element);

    ENTER;
    RETV(0);
}

/**
 *  namespace_body = '{' (namespace_element)* '}'
 *
 * @param node
 *
 */
int ast_namespace_body(AstNamespaceBody* node) {

    assert(node != NULL);
    assert(node->node.type == AST_namespace_body);

    ENTER;
    RETV(0);
}

/**
 *  namespace_definition = 'namespace' SYMBOL namespace_body
 * @param node
 *
 */
int ast_namespace_definition(AstNamespaceDefinition* node) {

    assert(node != NULL);
    assert(node->node.type == AST_namespace_definition);

    ENTER;
    RETV(0);
}

