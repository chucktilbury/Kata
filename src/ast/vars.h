/**
 * @file vars.h
 * 
 * @brief 
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __VARS_H__
#define __VARS_H__
#include "ast.h"

/**
 *  var_decl
 *      = type_name SYMBOL
 */
typedef struct _ast_var_decl_ {
    ast_node node;
} ast_var_decl;

/**
 *  var_decl_list
 *      = var_decl ( ',' var_decl )*
 */
typedef struct _ast_var_decl_list_ {
    ast_node node;
} ast_var_decl_list;

/**
 *  var_definition
 *      = var_decl ( '=' assignment_item )?
 */
typedef struct _ast_var_definition_ {
    ast_node node;
} ast_var_definition;


void traverse_var_decl(ast_var_decl* node);
void traverse_var_decl_list(ast_var_decl_list* node);
void traverse_var_definition(ast_var_definition* node);

#endif /* __VARS_H__ */
