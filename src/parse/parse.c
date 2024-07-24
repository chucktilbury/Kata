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
#include "trace.h"
#include "parse.h"
#include "scanner.h"
#include "paths.h"
#include "symbols.h"

// this is a temporary path for development otherwise, this would be received
// from configurations.
static const char* dummy_path[] = {
    "./",
    "../",
    "../test/",
    "../test/inputs/",
    "../test/misc",
    "../test/parse",
    "../test/stuff/",
    NULL
};


extern ParserState* parser_state;

/**
 * @brief Initialize the parser and open the first file.
 *
 * @param fname
 *
 */
void init_parser(const char* fname) {

    ENTER;

    TRACE("file name: \"%s\"", fname);
    init_paths(dummy_path);
    init_scanner(raw_string(find_file(fname)));
    init_symbol_table();
    init_global_context();
    create_parser_state();

    RET;
}

/**
 * @brief Public interface to the parser. This is also called by the import
 * functionality.
 *
 * @return ast_module*
 *
 */
ast_module* parse(void) {

    ENTER;
    ast_module* node = NULL;

    if(NULL == (node = parse_module())) {
        show_warning("module is empty");
    }

    RETV(node);
}

