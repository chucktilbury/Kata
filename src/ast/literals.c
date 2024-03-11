/**
 * @file literals.c
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
 *  scope_operator
 *      = 'private'
 *      / 'public'
 *      / 'protected'
 *      
 * @param node 
 * 
 */
void traverse_scope_operator(ast_scope_operator* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    AST_CALLBACK(func, node);
    TRACE_TERM(node->token);
    RET;
}

/**
 * @brief 
 * 
 *  literal_type_name
 *      = 'float'
 *      / 'unsigned'
 *      / 'signed'
 *      / 'string'
 *      / 'boolean'
 *      / 'nothing'
 *      / 'list'
 *      / 'dict'
 *      / 'function'
 *      
 * @param node 
 * 
 */
void traverse_literal_type_name(ast_literal_type_name* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    AST_CALLBACK(func, node);
    TRACE_TERM(node->token);
    RET;
}

/**
 * @brief 
 * 
 *  literal_value
 *      = LITERAL_FLOAT
 *      / LITERAL_UNSIGNED
 *      / LITERAL_SIGNED
 *      / LITERAL_BOOL
 *      
 * @param node 
 * 
 */
void traverse_literal_value(ast_literal_value* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    AST_CALLBACK(func, node);
    TRACE_TERM(node->token);
    RET;
}

/**
 * @brief 
 * 
 *  type_name
 *      = literal_type_name
 *      / compound_name
 *      
 * @param node 
 * 
 */
void traverse_type_name(ast_type_name* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    AST_CALLBACK(func, node);
    switch(ast_node_type(node->nterm)) {
        case AST_literal_type_name:
            traverse_literal_type_name((ast_literal_type_name*)node->nterm, func);
            break;
        case AST_compound_name:
            traverse_compound_name((ast_compound_name*)node->nterm, func);
            break;
        default:
            RAISE(TRAVERSE_ERROR, "unexpected node type in %s: %s", __func__, nterm_to_str(node->nterm));
    }
    RET;
}


