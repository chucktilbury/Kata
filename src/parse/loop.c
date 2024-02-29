/**
 * @file loop.c
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
#include "scanner.h"

/**
 * @brief
 *
 *  while_definition
 *      = 'while' ( '(' ( expression )? ')' )?
 *
 * @return ast_while_definition*
 *
 */
ast_while_definition* parse_while_definition() {

    ENTER;
    ast_while_definition* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  while_clause
 *      = while_definition function_body
 *
 * @return ast_while_clause*
 *
 */
ast_while_clause* parse_while_clause() {

    ENTER;
    ast_while_clause* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  do_clause
 *      = 'do' function_body while_definition
 *
 * @return ast_do_clause*
 *
 */
ast_do_clause* parse_do_clause() {

    ENTER;
    ast_do_clause* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  for_clause
 *      = 'for' ( '(' (type_name)? SYMBOL 'in' expression ')' )?
 *              function_body
 *
 * @return ast_for_clause*
 *
 */
ast_for_clause* parse_for_clause() {

    ENTER;
    ast_for_clause* node = NULL;

    RETV(node);
}


