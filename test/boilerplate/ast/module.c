/**
 * @file module.c
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
 *  module
 *      = ( module_item )+
 *      
 * @param node 
 * 
 */
void traverse_module(ast_module* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  module_item
 *      = namespace_item
 *      / 'import' formatted_strg AS SYMBOL
 *      / start_function
 *      
 * @param node 
 * 
 */
void traverse_module_item(ast_module_item* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  namespace_item
 *      = scope_operator
 *      / namespace_definition
 *      / class_definition
 *      / func_definition
 *      / ( 'const' )? var_definition
 *      
 * @param node 
 * 
 */
void traverse_namespace_item(ast_namespace_item* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  namespace_definition
 *      = 'namespace' SYMBOL '{' ( namespace_item )+ '}'
 *      
 * @param node 
 * 
 */
void traverse_namespace_definition(ast_namespace_definition* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  class_item
 *      = scope_operator
 *      / ( 'const' )? var_decl
 *      / ( 'virtual' )? func_decl
 *      
 * @param node 
 * 
 */
void traverse_class_item(ast_class_item* node) {

    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  class_definition
 *      = 'class' SYMBOL ( '(' ( compound_name )? ')' )?
 *              '{' ( class_item )+ '}'
 *      
 * @param node 
 * 
 */
void traverse_class_definition(ast_class_definition* node) {

    ENTER;
    RET;
}


