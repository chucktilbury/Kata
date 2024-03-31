/**
 * @file import.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-26-2024
 * @copyright Copyright (c) 2024
 */
//#define USE_TRACE 1
//#include "util.h"
#include "parse.h"
#include "scanner.h"

/**
 * @brief
 *
 *  import_statement
 *      = 'import' formatted_strg AS SYMBOL
 *
 * @return ast_import_statement*
 *
 */
ast_import_statement* parse_import_statement() {

    ENTER;
    ast_import_statement* node = NULL;

    RETV(node);
}
