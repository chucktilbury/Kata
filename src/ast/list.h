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
    LList list;
} ast_list_init;

/**
 *  dict_init_element
 *      = ( LITERAL_DSTRG / LITERAL_SSTRG ) ':' assignment_item
 */
typedef struct _ast_dict_init_element_ {
    ast_node node;
    Token* key;
    struct _ast_assignment_item_* item;
} ast_dict_init_element;

/**
 *  dict_init
 *      = '[' dict_init_element ( ',' dict_init_element )* ']'
 */
typedef struct _ast_dict_init_ {
    ast_node node;
    LList list;
} ast_dict_init;

/**
 *  array_param
 *      = '[' ( expression / string_expression ) ']'
 */
typedef struct _ast_array_param_ {
    ast_node node;
    ast_node* nterm;
} ast_array_param;

/**
 *  array_param_list
 *      = array_param (array_param)*
 */
typedef struct _ast_array_param_list_ {
    ast_node node;
    LList list;
} ast_array_param_list;

/**
 *  array_reference
 *      = SYMBOL array_param_list
 */
typedef struct _ast_array_reference_ {
    ast_node node;
    Token* symbol;
    struct _ast_array_param_list_* param;
} ast_array_reference;


void traverse_list_init(ast_list_init* node, PassFunc pre, PassFunc post);
void traverse_dict_init_element(ast_dict_init_element* node, PassFunc pre, PassFunc post);
void traverse_dict_init(ast_dict_init* node, PassFunc pre, PassFunc post);
void traverse_array_param(ast_array_param* node, PassFunc pre, PassFunc post);
void traverse_array_param_list(ast_array_param_list* node, PassFunc pre, PassFunc post);
void traverse_array_reference(ast_array_reference* node, PassFunc pre, PassFunc post);

#endif /* __LIST_H__ */
