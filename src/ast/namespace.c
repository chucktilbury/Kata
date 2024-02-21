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
void ast_namespace_element(AstNamespaceElement* node) {

    assert(node != NULL);
    assert(AST_namespace_element == get_ast_node_type((AstNode*)node));

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));
    switch(get_ast_node_type(node->elem)) {
        case AST_namespace_definition:
            ast_namespace_definition((AstNamespaceDefinition*)node->elem);
            break;
        case AST_type_spec:
            ast_type_spec((AstTypeSpec*)node->elem);
            break;
        case AST_scope_operator:
            ast_scope_operator((AstScopeOperator*)node->elem);
            break;
        default:
            RAISE(AST_TRAVERSE_ERROR,
                    "expected a namespace element, but got %s", n_to_str(node->elem));
    }
    RET;
}

/**
 *  namespace_body = '{' (namespace_element)* '}'
 *
 * @param node
 *
 */
void ast_namespace_body(AstNamespaceBody* node) {

    assert(node != NULL);
    assert(AST_namespace_body == get_ast_node_type((AstNode*)node));

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));

    PtrListIter* iter = init_ptr_list_iterator(node->lst);
    AstNode* n = NULL;
    while(NULL != (n = iterate_ptr_list(iter))) {
        if(AST_namespace_element == get_ast_node_type((AstNode*)n)) {
            ast_namespace_element((AstNamespaceElement*)n);
        }
        else {
            RAISE(AST_TRAVERSE_ERROR, "expected a namespace element but got %s", n_to_str(n));
            // break;
        }
    }

    RET;
}

/**
 *  namespace_definition = 'namespace' SYMBOL namespace_body
 * @param node
 *
 */
void ast_namespace_definition(AstNamespaceDefinition* node) {

    assert(node != NULL);
    assert(AST_namespace_definition == get_ast_node_type((AstNode*)node));

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));
    TRACE_TERM(node->name);
    if(AST_namespace_body == get_ast_node_type((AstNode*)node->body)) {
        ast_namespace_body((AstNamespaceBody*)node->body);
    }
    else {
        RAISE(AST_TRAVERSE_ERROR,
                "expected a namespace body but got %s", n_to_str((AstNode*)node->body));
        // break;
    }

    RET;
}

