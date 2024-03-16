/**
 * @file compound.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __COMPOUND_H__
#define __COMPOUND_H__
#include "ast.h"

/**
 *  compound_name
 *      = SYMBOL ( '.' SYMBOL )*
 */
typedef struct _ast_compound_name_ {
    ast_node node;
    LList list;
    Str* raw_name;
} ast_compound_name;


/**
 *  compound_name_list
 *      = '(' ( compound_name (',' compound_name)* )? ')'
 */
typedef struct _ast_compound_name_list_ {
    ast_node node;
    LList list;
} ast_compound_name_list;

/**
 *  compound_ref_item
 *      = SYMBOL
 *      / array_reference
 */
typedef struct _ast_compound_ref_item_ {
    ast_node node;
    Token* token;
    ast_node* nterm;
} ast_compound_ref_item;

/**
 *  compound_reference
 *      = compound_ref_item ( '.' compound_ref_item )*
 */
typedef struct _ast_compound_reference_ {
    ast_node node;
    LList list;
} ast_compound_reference;

void traverse_compound_name(ast_compound_name* node, PassFunc func);
void traverse_compound_name_list(ast_compound_name_list* node, PassFunc func);
void traverse_compound_ref_item(ast_compound_ref_item* node, PassFunc func);
void traverse_compound_reference(ast_compound_reference* node, PassFunc func);

#endif /* __COMPOUND_H__ */
