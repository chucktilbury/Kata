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
void ast_scope_operator(AstScopeOperator* node) {

    assert(node != NULL);
    assert(node->node.type == AST_scope_operator);

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));
    TRACE_TERM(node->tok);

    RET;
}

/**
 *  module_element
 *      = namespace_element
 *      | import_statement
 *
 * @param node
 *
 */
void ast_module_element(AstModuleElement* node) {

    assert(node != NULL);
    assert(node->node.type == AST_module_element);

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));

    AstType type = get_ast_node_type((AstNode*)node->elem);
    switch(type) {
        case AST_import_statement:
            ast_import_statement((AstImportStatement*)node->elem);
            break;
        case AST_namespace_element:
            ast_namespace_element((AstNamespaceElement*)node->elem);
            break;
        default:
            RAISE(AST_TRAVERSE_ERROR, "expected import statment or a "
                    "namespace element but got a %s", n_to_str((AstNode*)node->elem));
            break;
    }

    RET;
}

/**
 * module = (module_element)+
 *
 * @param node
 *
 */
void ast_module(AstModule* node) {

    assert(node != NULL);
    assert(node->node.type == AST_module);

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));
    PtrListIter* iter = init_ptr_list_iterator(node->lst);
    AstNode* n = NULL;

    while(NULL != (n = iterate_ptr_list(iter))) {
        if(AST_module_element == get_ast_node_type((AstNode*)n))
            ast_module_element((AstModuleElement*)n);
        else {
            RAISE(AST_TRAVERSE_ERROR, "expected module element but got a %s", n_to_str(n));
            //break;
        }
    }

    RET;
}

/**
 *  compound_name = SYMBOL ('.' SYMBOL)*
 *
 * @param node
 *
 */
void ast_compound_name(AstCompoundName* node) {

    assert(node != NULL);
    assert(node->node.type == AST_compound_name);

    ENTER;
    TRACE("NODE: %s", n_to_str((AstNode*)node));

    TRACE("RAW: %s", raw_string(node->str));
    PtrListIter* iter = init_ptr_list_iterator(node->lst);
    Token* tok = NULL;
    while(NULL != (tok = iterate_ptr_list(iter)))
        TRACE_TERM(tok);

    RET;
}

