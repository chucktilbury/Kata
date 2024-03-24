/**
 * @file module.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __MODULE_H__
#define __MODULE_H__
#include "ast.h"

/**
 *  module
 *      = ( module_item )+
 */
typedef struct _ast_module_ {
    ast_node node;
    LList list;
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
    LList list;
    ScopeType scope;
} ast_namespace_definition;

/**
 *  class_item
 *      = scope_operator
 *      / var_decl
 *      / func_decl
 */
typedef struct _ast_class_item_ {
    ast_node node;
    ast_node* nterm;
    ScopeType scope;
} ast_class_item;

/**
 *  class_definition
 *      = 'class' SYMBOL ( '(' ( type_name )? ')' )? '{' ( class_item )+ '}'
 */
typedef struct _ast_class_definition_ {
    ast_node node;
    Token* name;
    struct _ast_type_name_* parent;
    LList list;
    ScopeType scope;
} ast_class_definition;

void traverse_module(ast_module* node, PassFunc pre, PassFunc post);
void traverse_module_item(ast_module_item* node, PassFunc pre, PassFunc post);
void traverse_namespace_item(ast_namespace_item* node, PassFunc pre, PassFunc post);
void traverse_namespace_definition(ast_namespace_definition* node, PassFunc pre, PassFunc post);
void traverse_class_item(ast_class_item* node, PassFunc pre, PassFunc post);
void traverse_class_definition(ast_class_definition* node, PassFunc pre, PassFunc post);

#endif /* __MODULE_H__ */
