/**
 * @file compound.c
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
 *  compound_name
 *      = SYMBOL ( '.' SYMBOL )*
 *      
 * @return ast_compound_name* 
 * 
 */
ast_compound_name* parse_compound_name(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_compound_name* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  compound_ref_item
 *      = SYMBOL
 *      / array_reference
 *      
 * @return ast_compound_ref_item* 
 * 
 */
ast_compound_ref_item* parse_compound_ref_item(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_compound_ref_item* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  compound_reference
 *      = compound_ref_item ( '.' compound_ref_item )*
 *      
 * @return ast_compound_reference* 
 * 
 */
ast_compound_reference* parse_compound_reference(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_compound_reference* node = NULL;

    RETV(node);
}


