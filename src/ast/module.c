/**
 * @file module.c
 *
 * @brief Implement the functions that traverse the AST.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-11-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_ast.h"

/**
 *  scope_operator
 *      = PUBLIC
 *      | PRIVATE
 *      | PROTECTED
 *
 * @param node
 *
 */
int ast_scope_operator(AstScopeOperator* node) {

    assert(node != NULL);
    assert(node->node.type == AST_scope_operator);

    ENTER;
    RETV(0);
}

/**
 *  module_element
 *      = namespace_element
 *      | import_statement
 *
 * @param node
 *
 */
int ast_module_element(AstModuleElement* node) {

    assert(node != NULL);
    assert(node->node.type == AST_module_element);

    ENTER;
    TRACE("NODE: %s", n_to_str(node));
    PtrListIter* iter = init_ptr_list_iterator(node->lst);
    AstNode* n;

    AstType type = get_ast_node_type((AstNode*)n);
    switch(type) {
        case AST_import_statement:
            ast_import_statement((AstImportStatement*)n);
            break;
        case AST_namespace_element:
            ast_namespace_element((AstNamespaceElement*)n);
            break;
        default:
            RAISE(AST_TRAVERSE_ERROR, "expected import statment or a "
                    "namespace element but got a %s", n_to_str(n));
            break;
    }
    RETV(0);
}

/**
 * module = (module_element)+
 *
 * @param node
 *
 */
int ast_module(AstModule* node) {

    assert(node != NULL);
    assert(node->node.type == AST_module);

    ENTER;
    TRACE("NODE: %s", n_to_str(node));
    PtrListIter* iter = init_ptr_list_iterator(node->lst);
    AstNode* n;

    while(n = iterate_ptr_list(iter)) {
        AstType type = get_ast_node_type((AstNode*)n);
        switch(type) {
            case AST_import_statement:
                ast_import_statement((AstImportStatement*)n);
                break;
            case AST_namespace_element:
                ast_namespace_element((AstNamespaceElement*)n);
                break;
            default:
                RAISE(AST_TRAVERSE_ERROR, "expected import statment or a "
                        "namespace element but got a %s", n_to_str(n));
                break;
        }
    }
    RETV(0);
}

/**
 *  compound_name = SYMBOL ('.' SYMBOL)*
 *
 * @param node
 *
 */
int ast_compound_name(AstCompoundName* node) {

    assert(node != NULL);
    assert(node->node.type == AST_compound_name);

    ENTER;
    RETV(0);
}

