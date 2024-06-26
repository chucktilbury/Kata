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
 */
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
 *  function_declaration
 *      = 'func' ( 'virtual' )? SYMBOL type_name_list type_name_list
 */
typedef struct _ast_function_declaration_ {
    ast_node node;
    Token* name;
    bool is_virtual;
    struct _ast_type_name_list_* inputs;
    struct _ast_type_name_list_* outputs;
} ast_function_declaration;

/**
 *  create_declaration
 *      = 'func' ( 'virtual' )? 'create' type_name_list
 */
typedef struct _ast_create_declaration_ {
    ast_node node;
    bool is_virtual;
    struct _ast_type_name_list_* inputs;
} ast_create_declaration;

/**
 *  destroy_declaration
 *      = 'func' ( 'virtual' )? 'destroy'
 */
typedef struct _ast_destroy_declaration_ {
    ast_node node;
    bool is_virtual;
} ast_destroy_declaration;

/**
 *  function_definition
 *      = 'func' ( 'virtual' )? compound_name
 *          var_decl_list var_decl_list function_body
 */
typedef struct _ast_function_definition_ {
    ast_node node;
    bool is_virtual;
    struct _ast_compound_name_* name;
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
typedef struct _ast_create_name_ {
    ast_node node;
    LinkList* list;
} ast_create_name;

/**
 * A special rule is needed because of a conflict with compound_name
 *
 *  dtor_name
 *      = SYMBOL ('.' SYMBOL)? '.' 'destroy'
 */
typedef struct _ast_destroy_name_ {
    ast_node node;
    LinkList* list;
} ast_destroy_name;

/**
 *  create_definition
 *      = 'func' ( 'virtual' )? ctor_name var_decl_list function_body
 */
typedef struct _ast_create_definition_ {
    ast_node node;
    bool is_virtual;
    struct _ast_create_name_* name;
    struct _ast_var_decl_list_* inputs;
    struct _ast_function_body_* body;
} ast_create_definition;

/**
 *  destroy_definition
 *      = 'func' ( 'virtual' )? dtor_name function_body
 */
typedef struct _ast_destroy_definition_ {
    ast_node node;
    bool is_virtual;
    struct _ast_destroy_name_* name;
    struct _ast_function_body_* body;
} ast_destroy_definition;

/**
 *  function_body
 *      = '{' ( function_body_element )* '}'
 */
typedef struct _ast_function_body_ {
    ast_node node;
    LinkList* list;
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

void traverse_function_reference(ast_function_reference* node, PassFunc pre, PassFunc post);
void traverse_create_reference(ast_create_reference* node, PassFunc pre, PassFunc post);
void traverse_destroy_reference(ast_destroy_reference* node, PassFunc pre, PassFunc post);
void traverse_function_declaration(ast_function_declaration* node, PassFunc pre, PassFunc post);
void traverse_create_declaration(ast_create_declaration* node, PassFunc pre, PassFunc post);
void traverse_destroy_declaration(ast_destroy_declaration* node, PassFunc pre, PassFunc post);
void traverse_function_definition(ast_function_definition* node, PassFunc pre, PassFunc post);
void traverse_create_name(ast_create_name* node, PassFunc pre, PassFunc post);
void traverse_destroy_name(ast_destroy_name* node, PassFunc pre, PassFunc post);
void traverse_create_definition(ast_create_definition* node, PassFunc pre, PassFunc post);
void traverse_destroy_definition(ast_destroy_definition* node, PassFunc pre, PassFunc post);
void traverse_function_body(ast_function_body* node, PassFunc pre, PassFunc post);
void traverse_start_function(ast_start_function* node, PassFunc pre, PassFunc post);
void traverse_function_assignment(ast_function_assignment* node, PassFunc pre, PassFunc post);

#endif /* __FUNC_H__ */
