/**
 * @file ast.c
 *
 * @brief Implement the AST node functionality.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-15-2024
 * @copyright Copyright (c) 2024
 */
#include "ast.h"
#include "internal_ast.h"
#include "scanner.h"
#include "trace.h"

/**
 * @brief Create a ast node object. This actually should only be used with the
 * macro defined in ast.h. It allocates the full node size but returns it cast
 * back to AstNode so that it can be manipulated by various functions.
 *
 * @param type
 * @return AstNode*
 *
 */
AstNode* create_ast_node(AstType type, size_t size) {

    AstNode* node  = _ALLOC(size);
    node->type = type;

    return node;
}

/**
 * @brief Provides a portable and transparent way to get a node's type.
 *
 * @param node
 * @return AstType
 *
 */
AstType get_ast_node_type(AstNode* node) {

    assert(node != NULL);

    return node->type;
}

const char* n_to_str(AstNode* node) {

    AstType type = get_ast_node_type(node);

    return (type == AST_module)? "module" :
        (type == AST_scope_operator)? "scope_operator" :
        (type == AST_compound_name)? "compound_name" :
        (type == AST_module_element)? "module_element" :
        (type == AST_module_element_list)? "module_element_list" :
        (type == AST_namespace_element)? "namespace_element" :
        (type == AST_namespace_body)? "namespace_body" :
        (type == AST_namespace_definition)? "namespace_definition" :
        (type == AST_type_name)? "type_name" :
        (type == AST_type_spec)? "type_spec" :
        (type == AST_type_spec_element)? "type_spec_element" :
        (type == AST_import_statement)? "import_statement" :
        (type == AST_primary_expression)? "primary_expression" :
        (type == AST_expression_list)? "expression_list" :
        (type == AST_formatted_string)? "formatted_string" :
        (type == AST_expression)? "expression" : "UNKNOWN";
}

/**
 * @brief Traverse the tree and print out information about what is in the
 * tree and where it is.
 *
 * @param node
 *
 */
void traverse_ast(AstModule* tree) {

    assert(tree != NULL);
    ENTER;

    ast_module(tree);

    RET;

    //RETV(0);
}

/**
 * @brief Print the non-terminal symbol for the trace functionality.
 *
 * @param node
 *
 */
void print_nonterminal(AstNode* node) {

    assert(node != NULL);
}
