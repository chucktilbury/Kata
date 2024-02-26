/**
 * @file except.c
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
 *  try_clause
 *      = 'try' function_body except_clause
 * 
 * @param node 
 * 
 */
void traverse_try_clause(ast_try_clause* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  except_clause_mid
 *      = 'except' '(' SYMBOL ',' SYMBOL ')' function_body
 * 
 * @param node 
 * 
 */
void traverse_except_clause_mid(ast_except_clause_mid* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  except_clause_final
 *      = 'except' '(' 'any' ',' SYMBOL ')' function_body
 * 
 * @param node 
 * 
 */
void traverse_except_clause_final(ast_except_clause_final* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

/**
 * @brief 
 * 
 *  except_clause
 *      = ( except_clause_mid )+ ( except_clause_final )?
 *      / except_clause_final
 * 
 * @param node 
 * 
 */
void traverse_except_clause(ast_except_clause* node) {

    assert(node != NULL);
    
    ENTER;
    RET;
}

