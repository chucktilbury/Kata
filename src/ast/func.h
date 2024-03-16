/**
 * @file func.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __FUNC_H__
#define __FUNC_H__
#include "ast.h"

/**
 *  function_reference
 *      = compound_reference expression_list compound_name_list
\ */
typedef struct _ast_function_reference_ {
    ast_node node;
    struct _ast_compound_reference_* name;
    struct _ast_expression_list_* inp;
    struct _ast_compound_name_list_* outp;
} ast_function_reference;

/**
 *  create_reference
 *      = create_name expression_list
 */
typedef struct _ast_create_reference_ {
    ast_node node;
    struct _ast_create_name_* name;
    struct _ast_expression_list_* inp;
} ast_create_reference;

/**
 *  destroy_reference
 *      = destroy_name
 */
typedef struct _ast_destroy_reference_ {
    ast_node node;
    struct _ast_destroy_name_* name;
} ast_destroy_reference;

/**
 *  func_qualifier
 *      = ('virtual')?
 *      / ('function')?
 */
typedef struct _ast_func_qualifier_ {
    ast_node node;
    bool is_virtual;
} ast_func_qualifier;

/**
 *  function_declaration
 *      = (func_qualifier)? SYMBOL type_name_list type_name_list
 */
typedef struct _ast_function_declaration_ {
    ast_node node;
    Token* name;
    struct _ast_func_qualifier_* qual;
    struct _ast_type_name_list_* inputs;
    struct _ast_type_name_list_* outputs;
} ast_function_declaration;

/**
 *  create_declaration
 *      = (func_qualifier)? 'create' type_name_list
 */
typedef struct _ast_create_declaration_ {
    ast_node node;
    struct _ast_func_qualifier_* qual;
    struct _ast_type_name_list_* inputs;
} ast_create_declaration;

/**
 *  destroy_declaration
 *      = (func_qualifier)? 'destroy'
 */
typedef struct _ast_destroy_declaration_ {
    ast_node node;
    struct _ast_func_qualifier_* qual;
} ast_destroy_declaration;

/**
 *  function_definition
 *      = (func_qualifier)? compound_name
 *          var_decl_list var_decl_list function_body
 */
typedef struct _ast_function_definition_ {
    ast_node node;
    struct _ast_compound_reference_* name;
    struct _ast_func_qualifier_* qual;
    struct _ast_var_decl_list_* inputs;
    struct _ast_var_decl_list_* outputs;
    struct _ast_function_body_* body;
} ast_function_definition;

/**
 * A special rule is needed because of a conflict with compound_name
 *
 *  ctor_name
 *      = SYMBOL ('.' SYMBOL)? '.' 'create'
*/
typedef struct _ast_ctor_name_ {
    ast_node node;
    LList* list;
} ast_ctor_name;

/**
 * A special rule is needed because of a conflict with compound_name
 *
 *  dtor_name
 *      = SYMBOL ('.' SYMBOL)? '.' 'destroy'
*/
typedef struct _ast_dtor_name_ {
    ast_node node;
    LList* list;
} ast_dtor_name;

/**
 *  create_definition
 *      = (func_qualifier)? ctor_name var_decl_list function_body
 */
typedef struct _ast_create_definition_ {
    ast_node node;
    struct _ast_ctor_name_* name;
    struct _ast_func_qualifier_* qual;
    struct _ast_var_decl_list_* inputs;
    struct _ast_function_body_* body;
} ast_create_definition;

/**
 *  destroy_definition
 *      = (func_qualifier)? dtor_name function_body
 */
typedef struct _ast_destroy_definition_ {
    ast_node node;
    struct _ast_dtor_name_* name;
    struct _ast_func_qualifier_* qual;
    struct _ast_function_body_* body;
} ast_destroy_definition;

/**
 *  function_body
 *      = '{' ( function_body_element )* '}'
 */
typedef struct _ast_function_body_ {
    ast_node node;
    LList* list;
} ast_function_body;

/**
 *  start_function
 *      = ('function')? 'start' function_body
 */
typedef struct _ast_start_function_ {
    ast_node node;
    struct _ast_function_body_* body;
} ast_start_function;

/**
 *  function_assignment
 *      = compound_reference type_name_list type_name_list
 */
typedef struct _ast_function_assignment_ {
    ast_node node;
    struct _ast_compound_reference_* name;
    struct _ast_type_name_list_* inp;
    struct _ast_type_name_list_* outp;
} ast_function_assignment;

void traverse_function_reference(ast_function_reference* node, PassFunc func);
void traverse_create_reference(ast_create_reference* node, PassFunc func);
void traverse_destroy_reference(ast_destroy_reference* node, PassFunc func);
void traverse_func_qualifier(ast_func_qualifier* node, PassFunc func);
void traverse_function_declaration(ast_function_declaration* node, PassFunc func);
void traverse_create_declaration(ast_create_declaration* node, PassFunc func);
void traverse_destroy_declaration(ast_destroy_declaration* node, PassFunc func);
void traverse_function_definition(ast_function_definition* node, PassFunc func);
void traverse_ctor_name(ast_ctor_name* node, PassFunc func);
void traverse_dtor_name(ast_dtor_name* node, PassFunc func);
void traverse_create_definition(ast_create_definition* node, PassFunc func);
void traverse_destroy_definition(ast_destroy_definition* node, PassFunc func);
void traverse_function_body(ast_function_body* node, PassFunc func);
void traverse_start_function(ast_start_function* node, PassFunc func);
void traverse_function_assignment(ast_start_function* node, PassFunc func);

#endif /* __FUNC_H__ */
