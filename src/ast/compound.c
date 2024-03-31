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
#include <assert.h>

#include "link_list.h"
#include "trace.h"
#include "ast.h"
#include "errors.h"


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

    void* mark = NULL;
    while(NULL != (tok = iter_link_list(node->list, &mark))) {
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
    void* mark = NULL;
    while(NULL != (nterm = (ast_compound_name*)iter_link_list(node->list, &mark)))
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
        fatal_error("invalid compound reference item");

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

    void* mark = NULL;
    while(NULL != (nterm = iter_link_list(node->list, &mark))) {
        traverse_compound_ref_item(nterm, pre, post);
    }

    AST_CALLBACK(post, node);
    RET;
}

