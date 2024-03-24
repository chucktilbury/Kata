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
 *      = 'try' function_body except_clause
 */
typedef struct _ast_try_clause_ {
    ast_node node;
    struct _ast_function_body_* fbod;
    struct _ast_except_clause_* ecla;
} ast_try_clause;

/**
 *  except_clause_mid
 *      = 'except' '(' SYMBOL ',' SYMBOL ')' function_body
 */
typedef struct _ast_except_clause_mid_ {
    ast_node node;
    Token* name;
    Token* msg;
    struct _ast_function_body_* fbod;
} ast_except_clause_mid;

/**
 *  except_clause_final
 *      = 'except' '(' 'any' ',' SYMBOL ')' function_body
 */
typedef struct _ast_except_clause_final_ {
    ast_node node;
    Token* msg;
    struct _ast_function_body_* fbod;
} ast_except_clause_final;

/**
 *  except_clause
 *      = ( except_clause_mid )+ ( except_clause_final )?
 *      / except_clause_final
 */
typedef struct _ast_except_clause_ {
    ast_node node;
    LList* list;
    struct _ast_except_clause_final_* fin;
} ast_except_clause;

void traverse_try_clause(ast_try_clause* node, PassFunc pre, PassFunc post);
void traverse_except_clause_mid(ast_except_clause_mid* node, PassFunc pre, PassFunc post);
void traverse_except_clause_final(ast_except_clause_final* node, PassFunc pre, PassFunc post);
void traverse_except_clause(ast_except_clause* node, PassFunc pre, PassFunc post);

#endif /* __EXCEPT_H__ */
