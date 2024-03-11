/**
 * @file list.c
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
 *  list_init
 *      = '[' assignment_item ( ',' assignment_item )* ']'
 *      
 * @param node 
 * 
 */
void traverse_list_init(ast_list_init* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    AST_CALLBACK(func, node);
    RET;
}

/**
 * @brief 
 * 
 *  dict_init_element
 *      = ( LITERAL_DSTRG / LITERAL_SSTRG ) ':' assignment_item
 *      
 * @param node 
 * 
 */
void traverse_dict_init_element(ast_dict_init_element* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    AST_CALLBACK(func, node);
    RET;
}

/**
 * @brief 
 * 
 *  dict_init
 *      = '[' dict_init_element ( ',' dict_init_element )* ']'
 *      
 * @param node 
 * 
 */
void traverse_dict_init(ast_dict_init* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    AST_CALLBACK(func, node);
    RET;
}

/**
 * @brief 
 * 
 *  array_param
 *      = '[' ( expression / string_expression ) ']' ( array_param )*
 *      
 * @param node 
 * 
 */
void traverse_array_param(ast_array_param* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    AST_CALLBACK(func, node);
    RET;
}

/**
 * @brief 
 * 
 *  array_reference
 *      = SYMBOL array_param
 *      
 * @param node 
 * 
 */
void traverse_array_reference(ast_array_reference* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    AST_CALLBACK(func, node);
    RET;
}


