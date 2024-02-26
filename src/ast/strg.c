/**
 * @file string.c
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
 *  formatted_strg
 *      = LITERAL_DSTRG (expression_list)?
 *      
 * @param node 
 * 
 */
void traverse_formatted_strg(ast_formatted_strg* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  string_literal
 *      = LITERAL_SSTRG
 *      / formatted_strg
 *      
 * @param node 
 * 
 */
void traverse_string_literal(ast_string_literal* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  string_expr_item
 *      = string_literal
 *      / compound_reference
 *      / literal_value
 *      
 * @param node 
 * 
 */
void traverse_string_expr_item(ast_string_expr_item* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  string_expr
 *      = string_expr_item ( '+' string_expr_item )*
 *      
 * @param node 
 * 
 */
void traverse_string_expr(ast_string_expr* node) {

    ENTER;
    RET;
}


