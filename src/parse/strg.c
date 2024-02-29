/**
 * @file string.c
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
#include "scanner.h"

/**
 * @brief 
 * 
 *  string_literal
 *      = LITERAL_SSTRG
 *      / formatted_strg
 *      
 * @return ast_string_literal* 
 * 
 */
ast_string_literal* parse_string_literal(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_string_literal* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  string_expr_item
 *      = string_literal
 *      / compound_reference
 *      / literal_value
 *      
 * @return ast_string_expr_item* 
 * 
 */
ast_string_expr_item* parse_string_expr_item(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_string_expr_item* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  string_expr
 *      = string_expr_item ( '+' string_expr_item )*
 *      
 * @return ast_string_expr* 
 * 
 */
ast_string_expr* parse_string_expr(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_string_expr* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  formatted_strg
 *      = LITERAL_DSTRG (expression_list)?
 *      
 * @return ast_formatted_strg* 
 * 
 */
ast_formatted_strg* parse_formatted_strg(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_formatted_strg* node = NULL;

    RETV(node);
}

