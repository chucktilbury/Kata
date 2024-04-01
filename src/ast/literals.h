/**
 * @file literals.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __LITERALS_H__
#define __LITERALS_H__
#include <stdint.h>

#include "ast.h"
#include "link_list.h"

/**
 *  scope_operator
 *      = 'private'
 *      / 'public'
 *      / 'protected'
 */
typedef struct _ast_scope_operator_ {
    ast_node node;
    Token* token;
} ast_scope_operator;

/**
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
 */
typedef struct _ast_literal_type_name_ {
    ast_node node;
    Token* token;
} ast_literal_type_name;

/**
 *  literal_value
 *      = LITERAL_FLOAT
 *      / LITERAL_UNSIGNED
 *      / LITERAL_SIGNED
 *      / LITERAL_BOOL
 */
typedef struct _ast_literal_value_ {
    ast_node node;
    Token* token;
    union {
        double fnum;
        uint64_t unum;
        int64_t snum;
        bool bval;
    } value;
} ast_literal_value;

/**
 *  type_name
 *      = literal_type_name
 *      / compound_name
 */
typedef struct _ast_type_name_ {
    ast_node node;
    ast_node* nterm;
} ast_type_name;

/**
 *  type_name_list
 *      = '(' ( type_name (',' type_name)* )? ')'
 */
typedef struct _ast_type_name_list_ {
    ast_node node;
    LinkList* list;
} ast_type_name_list;

/**
 * This node type is created when an error is detected. It's just a
 * place holder and no emitter passes can be taken. There is no grammar
 * rule to represent.
 */
typedef struct _ast_error_ {
    ast_node node;
} ast_error;

void traverse_scope_operator(ast_scope_operator* node, PassFunc pre, PassFunc post);
void traverse_literal_type_name(ast_literal_type_name* node, PassFunc pre, PassFunc post);
void traverse_literal_value(ast_literal_value* node, PassFunc pre, PassFunc post);
void traverse_type_name(ast_type_name* node, PassFunc pre, PassFunc post);
void traverse_type_name_list(ast_type_name_list* node, PassFunc pre, PassFunc post);


#endif /* __LITERALS_H__ */
