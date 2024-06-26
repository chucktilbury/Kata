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
#include "strings.h"
/**
 *  compound_name
 *      = SYMBOL ( '.' SYMBOL )*
 */
typedef struct _ast_compound_name_ {
    ast_node node;
    LinkList* list;
    String* raw_name;
} ast_compound_name;


/**
 *  compound_name_list
 *      = '(' ( compound_name (',' compound_name)* )? ')'
 */
typedef struct _ast_compound_name_list_ {
    ast_node node;
    LinkList* list;
} ast_compound_name_list;

/**
 *  compound_ref_item
 *      = SYMBOL ( array_param_list )?
 */
typedef struct _ast_compound_ref_item_ {
    ast_node node;
    Token* symb;
    struct _ast_array_param_list_* apar;
} ast_compound_ref_item;

/**
 *  compound_reference
 *      = compound_ref_item ( '.' compound_ref_item )*
 */
typedef struct _ast_compound_reference_ {
    ast_node node;
    LinkList* list;
} ast_compound_reference;

void traverse_compound_name(ast_compound_name* node, PassFunc pre, PassFunc post);
void traverse_compound_name_list(ast_compound_name_list* node, PassFunc pre, PassFunc post);
void traverse_compound_ref_item(ast_compound_ref_item* node, PassFunc pre, PassFunc post);
void traverse_compound_reference(ast_compound_reference* node, PassFunc pre, PassFunc post);

#endif /* __COMPOUND_H__ */
