/**
 * @file string.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __STRG_H__
#define __STRG_H__
#include "ast.h"

/**
 *  formatted_strg
 *      = LITERAL_DSTRG (expression_list)?
 */
typedef struct _ast_formatted_strg_ {
    ast_node node;
    Token* str;
    struct _ast_expression_list_* exprs;
} ast_formatted_strg;

/**
 *  string_literal
 *      = LITERAL_SSTRG
 *      / formatted_strg
 */
typedef struct _ast_string_literal_ {
    ast_node node;
    Token* literal;
    struct _ast_formatted_strg_* fmt;
} ast_string_literal;

/**
 *  string_expr_item
 *      = string_literal
 *      / compound_reference
 *      / literal_value
 */
typedef struct _ast_string_expr_item_ {
    ast_node node;
    ast_node* nterm;
} ast_string_expr_item;

/**
 *  string_expr
 *      = string_expr_item ( '+' string_expr_item )*
 */
typedef struct _ast_string_expr_ {
    ast_node node;
    LinkList* list;
} ast_string_expr;


void traverse_formatted_strg(ast_formatted_strg* node, PassFunc pre, PassFunc post);
void traverse_string_literal(ast_string_literal* node, PassFunc pre, PassFunc post);
void traverse_string_expr_item(ast_string_expr_item* node, PassFunc pre, PassFunc post);
void traverse_string_expr(ast_string_expr* node, PassFunc pre, PassFunc post);


#endif /* __STRG_H__ */
