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
#include "trace.h"
#include "parse.h"
#include "scanner.h"

/**
 * @brief The import statement is like a namespace except that it opens a new
 * file and reads from that instead of the current file. When the imported
 * file is finished, then the input stream automatically switches back to the
 * previous file. When an imported file is being read, only the new symbols
 * are placed into the AST so that they can be checked when the symbol resolver
 * pass is entered.
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
