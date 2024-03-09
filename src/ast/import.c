/**
 * @file import.c
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
#include "ast.h"

/**
 * @brief 
 * 
 *  import_statement
 *      = 'import' formatted_string 'as' SYMBOL
 * 
 * @param node 
 * 
 */
void traverse_import_statement(ast_import_statement* node, PassFunc func) {

    assert(node != NULL);
    assert(func != NULL);
    
    ENTER;
    PASS_FUNC(func, node);
    RET;
}
