/**
 * @file literals.c
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

/**
 * @brief 
 * 
 *  scope_operator
 *      = 'private'
 *      / 'public'
 *      / 'protected'
 *      
 * @return ast_scope_operator* 
 * 
 */
ast_scope_operator* parse_scope_operator(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_scope_operator* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
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
 *      
 * @return ast_literal_type_name* 
 * 
 */
ast_literal_type_name* parse_literal_type_name(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_literal_type_name* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  literal_value
 *      = LITERAL_FLOAT
 *      / LITERAL_UNSIGNED
 *      / LITERAL_SIGNED
 *      / LITERAL_BOOL
 *      
 * @return ast_literal_value* 
 * 
 */
ast_literal_value* parse_literal_value(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_literal_value* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  type_name
 *      = literal_type_name
 *      / compound_name
 *      
 * @return ast_type_name* 
 * 
 */
ast_type_name* parse_type_name(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_type_name* node = NULL;

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


