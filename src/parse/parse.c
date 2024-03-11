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

extern ParserState* parser_state;

const char* scope_name(ScopeType type) {

    return (type == SCOPE_PUB)? "PUBLIC": 
            (type == SCOPE_PRIV)? "PRIVATE": 
            (type == SCOPE_PROT)? "PROTECTED": "UNKNOWN" ;

}

/**
 * @brief Initialze the parser and open the first file.
 *
 * @param fname
 *
 */
void init_parser(const char* fname) {

    ENTER;
    TRACE("file name: \"%s\"", fname);
    init_scanner(fname);
    parser_state = _ALLOC_T(ParserState);
    parser_state->scope = SCOPE_PRIV;
    parser_state->is_import = false;
    RET;
}

/**
 * @brief Public interface to the parser. This is also called by the import
 * functionality.
 *
 * @return ast_module*
 *
 */
ast_module* parse() {

    ENTER;
    ast_module* node = NULL;

    if(NULL != (node = parse_module())) {
        if(TOK_END_OF_INPUT != TTYPE) {
            EXPECTED("end of input");
            node = NULL;
        }
    }
    else {
        show_syntax_error("module cannot be empty");
    }

    RETV(node);
}
