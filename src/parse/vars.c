/**
 * @file vars.c
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
 *  var_decl
 *      = ( 'const' )? type_name SYMBOL
 *      
 * @return ast_var_decl* 
 * 
 */
ast_var_decl* parse_var_decl(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_var_decl* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  var_decl_list
 *      = var_decl ( ',' var_decl )*
 *      
 * @return ast_var_decl_list* 
 * 
 */
ast_var_decl_list* parse_var_decl_list(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_var_decl_list* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  var_definition
 *      = var_decl ( '=' assignment_item )?
 *      
 * @return ast_var_definition* 
 * 
 */
ast_var_definition* parse_var_definition(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_var_definition* node = NULL;

    RETV(node);
}



