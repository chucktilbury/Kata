/**
 * @file module.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _MODULE_H_
#define _MODULE_H_
#include "ast.h"

/**
 *  module
 *      = ( module_item )+
 */
typedef struct _ast_module_ {
    ast_node node;
    LinkList* list;
} ast_module;

/**
 *  module_item
 *      = namespace_item
 *      / 'import' formatted_strg AS SYMBOL
 *      / start_function
 */
typedef struct _ast_module_item_ {
    ast_node node;
    ast_node* nterm;
    ScopeType scope;
} ast_module_item;

/**
 *  namespace_item
 *      = scope_operator
 *      / namespace_definition
 *      / class_definition
 *      / func_definition
 *      / var_definition
 */
typedef struct _ast_namespace_item_ {
    ast_node node;
    ast_node* nterm;
    ScopeType scope;
} ast_namespace_item;

/**
 *  namespace_definition
 *      = 'namespace' SYMBOL '{' ( namespace_item )+ '}'
 */
typedef struct _ast_namespace_definition_ {
    ast_node node;
    Token* name;
    LinkList* list;
    ScopeType scope;
    SymContext* context;
} ast_namespace_definition;

/**
 *  class_item
 *      = scope_operator
 *      / class_var_declaration
 *      / function_declaration
 *      / create_declaration
 *      / destroy_declaration
 */
typedef struct _ast_class_item_ {
    ast_node node;
    ast_node* nterm;
    ScopeType scope;
} ast_class_item;

/**
 *  class_var_declaration
 *      ( 'var' / 'variable' ) var_decl
 */
typedef struct _ast_class_var_declaration_ {
    ast_node node;
    struct _ast_var_decl_* var;
} ast_class_var_declaration;

/**
 *  class_definition
 *      = 'class' SYMBOL ( '(' ( type_name )? ')' )? '{' ( class_item )+ '}'
 */
typedef struct _ast_class_definition_ {
    ast_node node;
    Token* name;
    struct _ast_type_name_* parent;
    LinkList* list;
    ScopeType scope;
    SymContext* context;
} ast_class_definition;

void traverse_module(ast_module* node, PassFunc pre, PassFunc post);
void traverse_module_item(ast_module_item* node, PassFunc pre, PassFunc post);
void traverse_namespace_item(ast_namespace_item* node, PassFunc pre, PassFunc post);
void traverse_namespace_definition(ast_namespace_definition* node, PassFunc pre, PassFunc post);
void traverse_class_item(ast_class_item* node, PassFunc pre, PassFunc post);
void traverse_class_definition(ast_class_definition* node, PassFunc pre, PassFunc post);
void traverse_class_var_declaration(ast_class_var_declaration* node, PassFunc pre, PassFunc post);

#endif /* _MODULE_H_ */
