/**
 * @file except.h
 *
 * @brief
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-25-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __EXCEPT_H__
#define __EXCEPT_H__
#include "ast.h"

/**
 *  try_clause
 *      = 'try' function_body ( except_clause )* ( final_clause )?
 */
typedef struct _ast_try_clause_ {
    ast_node node;
    struct _ast_function_body_* fbod;
    struct _ast_final_clause_* final;
    LinkList* except_list;
} ast_try_clause;

/**
 *  except_clause
 *      = 'except' '(' SYMBOL ',' SYMBOL ')' function_body
 */
typedef struct _ast_except_clause_ {
    ast_node node;
    Token* name;
    Token* msg;
    struct _ast_function_body_* fbod;
} ast_except_clause;

/**
 *  final_clause
 *      = 'final' '(' SYMBOL ')' function_body
 */
typedef struct _ast_final_clause_ {
    ast_node node;
    Token* msg;
    struct _ast_function_body_* fbod;
} ast_final_clause;


void traverse_try_clause(ast_try_clause* node, PassFunc pre, PassFunc post);
void traverse_except_clause(ast_except_clause* node, PassFunc pre, PassFunc post);
void traverse_final_clause(ast_final_clause* node, PassFunc pre, PassFunc post);

#endif /* __EXCEPT_H__ */
