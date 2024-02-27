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
 * @param node 
 * 
 */
void traverse_compound_name(ast_compound_name* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);

    ENTER;
    (*func)((ast_node*)node);
    RET;
}

/**
 * @brief 
 * 
 * @param node 
 * 
 */
void traverse_compound_ref_item(ast_compound_ref_item* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
    RET;
}

/**
 * @brief 
 * 
 * @param node 
 * 
 */
void traverse_compound_reference(ast_compound_reference* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    (*func)((ast_node*)node);
    RET;
}

