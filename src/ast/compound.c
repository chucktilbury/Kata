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
void traverse_compound_name(ast_compound_name* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    TRACE("raw_name: %s", raw_string(node->raw_name));
    PtrListIter* iter = init_ptr_list_iterator(node->list);
    Token* tok;
    while(NULL != (tok = iterate_ptr_list(iter))) {
        TRACE_TERM(tok);
    }
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
void traverse_compound_ref_item(ast_compound_ref_item* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
    if(node->token != NULL)
        TRACE_TERM(node->token);
    else if(node->nterm != NULL)
        traverse_array_reference((ast_array_reference*)node->nterm, func);
    else 
        RAISE(TRAVERSE_ERROR, "invalid compound reference item");
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
void traverse_compound_reference(ast_compound_reference* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
    PtrListIter* iter = init_ptr_list_iterator(node->list);
    ast_compound_ref_item* nterm;
    while(NULL != (nterm = iterate_ptr_list(iter))) {
        traverse_compound_ref_item(nterm, func);
    }
    RET;
}

