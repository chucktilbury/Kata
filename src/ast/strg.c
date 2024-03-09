/**
 * @file string.c
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
 *  formatted_strg
 *      = LITERAL_DSTRG (expression_list)?
 *      
 * @param node 
 * 
 */
void traverse_formatted_strg(ast_formatted_strg* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    PASS_FUNC(func, node);
    TRACE_TERM(node->str);
    TRACE("expr list len: %d", len_llist(node->exprs));
    traverse_expression_list(node->exprs, func);
    RET;
}

/**
 * @brief 
 * 
 *  string_literal
 *      = LITERAL_SSTRG
 *      / formatted_strg
 *      
 * @param node 
 * 
 */
void traverse_string_literal(ast_string_literal* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    PASS_FUNC(func, node);
    if(node->literal != NULL)
        TRACE_TERM(node->literal);
    else
        traverse_formatted_strg(node->fmt, func);
    RET;
}

/**
 * @brief 
 * 
 *  string_expr_item
 *      = string_literal
 *      / compound_reference
 *      / literal_value
 *      
 * @param node 
 * 
 */
void traverse_string_expr_item(ast_string_expr_item* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    PASS_FUNC(func, node);
    switch(ast_node_type(node->nterm)) {
        case AST_string_literal:
            traverse_string_literal((ast_string_literal*)node->nterm, func);
            break;
        case AST_compound_reference:
            traverse_compound_reference((ast_compound_reference*)node->nterm, func);
            break;
        case AST_literal_value:
            traverse_literal_value((ast_literal_value*)node->nterm, func);
            break;
        default:
            RAISE(TRAVERSE_ERROR, "unexpected node type in %s: %d",
                    __func__, ast_node_type(node->nterm));
    }
    RET;
}

/**
 * @brief 
 * 
 *  string_expr
 *      = string_expr_item ( '+' string_expr_item )*
 *      
 * @param node 
 * 
 */
void traverse_string_expr(ast_string_expr* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    PASS_FUNC(func, node);
    ast_string_expr_item* item;

    TRACE("num expr items: %d", len_llist(node->list));
    init_llist_iter(node->list);
    while(NULL != (item = (ast_string_expr_item*)iter_llist(node->list)))
        traverse_string_expr_item(item, func);

    RET;
}


