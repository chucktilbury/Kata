/**
 * @file literals.h
 * 
 * @brief 
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __LITERALS_H__
#define __LITERALS_H__
#include "ast.h"

/**
 *  scope_operator
 *      = 'private'
 *      / 'public'
 *      / 'protected'
 */
typedef struct _ast_scope_operator_ {
    ast_node node;
} ast_scope_operator;

/**
 *  literal_type_name
 *      = 'float'
 *      / 'unsigned'
 *      / 'signed'
 *      / 'string'
 *      / 'boolean'
 *      / 'nothing'
 *      / 'list'
 *      / 'dict'
 *      / 'function'
 */
typedef struct _ast_literal_type_name_ {
    ast_node node;
} ast_literal_type_name;

/**
 *  literal_value
 *      = LITERAL_FLOAT
 *      / LITERAL_UNSIGNED
 *      / LITERAL_SIGNED
 *      / LITERAL_BOOL
 */
typedef struct _ast_literal_value_ {
    ast_node node;
} ast_literal_value;

/**
 *  type_name
 *      = literal_type_name
 *      / compound_name
 */
typedef struct _ast_type_name_ {
    ast_node node;
} ast_type_name;

void traverse_scope_operator(ast_scope_operator* node);
void traverse_literal_type_name(ast_literal_type_name* node);
void traverse_literal_value(ast_literal_value* node);
void traverse_type_name(ast_type_name* node);


#endif /* __LITERALS_H__ */
