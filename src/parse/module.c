/**
 * @file module.c
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
 *  module
 *      = ( module_item )+
 *      
 * @return ast_module* 
 * 
 */
ast_module* parse_module(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_module* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  module_item
 *      = namespace_item
 *      / import_statement
 *      / start_function
 *      
 * @return ast_module_item* 
 * 
 */
ast_module_item* parse_module_item(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_module_item* node = NULL;

    RETV(node);
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
 * @return ast_namespace_item* 
 * 
 */
ast_namespace_item* parse_namespace_item(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_namespace_item* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  namespace_definition
 *      = 'namespace' SYMBOL '{' ( namespace_item )+ '}'
 *      
 * @return ast_namespace_definition* 
 * 
 */
ast_namespace_definition* parse_namespace_definition(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_namespace_definition* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  class_item
 *      = scope_operator
 *      / ( 'const' )? var_decl
 *      / ( 'virtual' )? func_decl
 *      
 * @return ast_class_item* 
 * 
 */
ast_class_item* parse_class_item(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_class_item* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  class_definition
 *      = 'class' SYMBOL ( '(' ( type_name )? ')' )?
 *              '{' ( class_item )+ '}'
 *      
 * @return ast_class_definition* 
 * 
 */
ast_class_definition* parse_class_definition(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    ast_class_definition* node = NULL;

    RETV(node);
}


