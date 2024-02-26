/**
 * @file string.h
 * 
 * @brief 
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __STRING_H__
#define __STRING_H__
#include "ast.h"

/**
 *  formatted_strg
 *      = LITERAL_DSTRG (expression_list)?
 */
typedef struct _ast_formatted_strg_ {
    ast_node node;
} ast_formatted_strg;

/**
 *  string_literal
 *      = LITERAL_SSTRG
 *      / formatted_strg
 */
typedef struct _ast_string_literal_ {
    ast_node node;
} ast_string_literal;

/**
 *  string_expr_item
 *      = string_literal
 *      / compound_reference
 *      / literal_value
 */
typedef struct _ast_string_expr_item_ {
    ast_node node;
} ast_string_expr_item;

/**
 *  string_expr
 *      = string_expr_item ( '+' string_expr_item )*
 */
typedef struct _ast_string_expr_ {
    ast_node node;
} ast_string_expr;




#endif /* __STRING_H__ */
