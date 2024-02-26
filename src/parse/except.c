/**
 * @file except.c
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
 *  try_clause
 *      = 'try' function_body except_clause
 *      
 * @return ast_try_clause* 
 * 
 */
ast_try_clause* parse_try_clause() {

    ENTER;
    ast_try_clause* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  except_clause_mid
 *      = 'except' '(' SYMBOL ',' SYMBOL ')' function_body
 *      
 * @return ast_except_clause_mid* 
 * 
 */
ast_except_clause_mid* parse_except_clause_mid() {

    ENTER;
    ast_except_clause_mid* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  except_clause_final
 *      = 'except' '(' 'any' ',' SYMBOL ')' function_body
 *      
 * @return ast_except_clause_final* 
 * 
 */
ast_except_clause_final* parse_except_clause_final() {

    ENTER;
    ast_except_clause_final* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  except_clause
 *      = ( except_clause_mid )+ ( except_clause_final )?
 *      / except_clause_final
 *      
 * @return ast_except_clause* 
 * 
 */
ast_except_clause* parse_except_clause() {

    ENTER;
    ast_except_clause* node = NULL;

    RETV(node);
}


