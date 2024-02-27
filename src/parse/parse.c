/**
 * @file parse.c
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
 * @brief Initialze the parser and open the first file.
 *
 * @param fname
 *
 */
ParserState* init_parser(const char* fname) {

    ENTER;
    init_scanner(fname);
    ParserState* state = _ALLOC_T(ParserState);

    RETV(state);
}

/**
 * @brief Public interface to the parser. This is also called by the import
 * functionality.
 *
 * @return ast_module*
 *
 */
ast_module* parse(ParserState* state) {

    assert(state != NULL);
    
    ENTER;
    RETV(NULL);
}
