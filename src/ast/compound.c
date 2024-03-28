/**
 * @file compound.c
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
 *  compound_name
 *      = SYMBOL ( '.' SYMBOL )*
 *
 * @param node
 *
 */
void traverse_compound_name(ast_compound_name* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);
    TRACE("raw_name: %s", raw_string(node->raw_name));
    Token* tok;

    init_llist_iter(node->list);
    while(NULL != (tok = iter_llist(node->list))) {
        TRACE_TERM(tok);
    }
    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  compound_name_list
 *      = '(' ( compound_name (',' compound_name)* )? ')'
 *
 * @param node
 * @param pre
 *
 */
void traverse_compound_name_list(ast_compound_name_list* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);

    ast_compound_name* nterm;
    init_llist_iter(node->list);
    while(NULL != (nterm = (ast_compound_name*)iter_llist(node->list)))
        traverse_compound_name(nterm, pre, post);

    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  compound_ref_item
 *      = SYMBOL
 *      / array_reference
 *
 * @param node
 *
 */
void traverse_compound_ref_item(ast_compound_ref_item* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);
    if(node->token != NULL)
        TRACE_TERM(node->token);
    else if(node->nterm != NULL)
        traverse_array_reference((ast_array_reference*)node->nterm, pre, post);
    else
        RAISE(TRAVERSE_ERROR, "invalid compound reference item");
    AST_CALLBACK(post, node);
    RET;
}

/**
 * @brief
 *
 *  compound_reference
 *      = compound_ref_item ( '.' compound_ref_item )*
 *
 * @param node
 *
 */
void traverse_compound_reference(ast_compound_reference* node, PassFunc pre, PassFunc post) {

    assert(node != NULL);

    ENTER;
    AST_CALLBACK(pre, node);
    ast_compound_ref_item* nterm;

    init_llist_iter(node->list);
    while(NULL != (nterm = iter_llist(node->list))) {
        traverse_compound_ref_item(nterm, pre, post);
    }
    AST_CALLBACK(post, node);
    RET;
}

