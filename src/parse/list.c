/**
 * @file list.c
 * 
 * @brief 
 * 
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-26-2024
 * @copyright Copyright (c) 2024
 */
#define USE_TRACE 1
#include "util.h"
#include "parse.h"

/**
 * @brief 
 * 
 *  list_init
 *      = '[' assignment_item ( ',' assignment_item )* ']'
 *      
 * @return ast_list_init* 
 * 
 */
ast_list_init* parse_list_init() {

    ENTER;
    ast_list_init* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  dict_init_element
 *      = ( LITERAL_DSTRG / LITERAL_SSTRG ) ':' assignment_item
 *      
 * @return ast_dict_init_element* 
 * 
 */
ast_dict_init_element* parse_dict_init_element() {

    ENTER;
    ast_dict_init_element* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  dict_init
 *      = '[' dict_init_element ( ',' dict_init_element )* ']'
 *      
 * @return ast_dict_init* 
 * 
 */
ast_dict_init* parse_dict_init() {

    ENTER;
    ast_dict_init* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  array_param
 *      = '[' ( expression / string_expression ) ']' ( array_param )*
 *      
 * @return ast_array_param* 
 * 
 */
ast_array_param* parse_array_param() {

    ENTER;
    ast_array_param* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  array_reference
 *      = SYMBOL array_param
 *      
 * @return ast_array_reference* 
 * 
 */
ast_array_reference* parse_array_reference() {

    ENTER;
    ast_array_reference* node = NULL;

    RETV(node);
}


