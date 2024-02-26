/**
 * @file list.h
 * 
 * @brief 
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __LIST_H__
#define __LIST_H__
#include "ast.h"

/**
 *  list_init
 *      = '[' assignment_item ( ',' assignment_item )* ']'
 */
typedef struct _ast_list_init_ {
    ast_node node;
} ast_list_init;

/**
 *  dict_init_element
 *      = ( LITERAL_DSTRG / LITERAL_SSTRG ) ':' assignment_item
 */
typedef struct _ast_dict_init_element_ {
    ast_node node;
} ast_dict_init_element;

/**
 *  dict_init
 *      = '[' dict_init_element ( ',' dict_init_element )* ']'
 */
typedef struct _ast_dict_init_ {
    ast_node node;
} ast_dict_init;

/**
 *  array_param
 *      = '[' ( expression / string_expression ) ']' ( array_param )*
 */
typedef struct _ast_array_param_ {
    ast_node node;
} ast_array_param;

/**
 *  array_reference
 *      = SYMBOL array_param
 */
typedef struct _ast_array_reference_ {
    ast_node node;
} ast_array_reference;


void traverse_list_init(ast_list_init* node);
void traverse_dict_init_element(ast_dict_init_element* node);
void traverse_dict_init(ast_dict_init* node);
void traverse_array_param(ast_array_param* node);
void traverse_array_reference(ast_array_reference* node);

#endif /* __LIST_H__ */
