/**
 * @file import.c
 *
 * @brief This is the import module. There is only one production, but
 * there is a lot in it. This production creates a namespace in the AST. It
 * does not, however, read the whole file. Only the symbols are read and
 * the actual implementation is expected to be linked as a separate module.
 *
 * Syntax:
 *      "import" formatted_string ("as" SYMBOL)?
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-05-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_parser.h"

/**
 * @brief Actually open the input file so that parsing can begin on it.
 *
 * @param str
 * @return true
 * @return false
 *
 */
static bool do_import(AstFormattedString* fname, Str* symbol) {

    ENTER;
    (void)symbol;
    // Scan a search path of locations to find the actual file in.

    // Open the actual file.
    open_file(raw_string(fname->strg->str));

    // convert the file name into a namespace entry

    // do_namespace() is defined in parser.c
    // push_namespace(symbol);

    RETV(true);
}

/**
 * @brief Handle the parsing of the import statement and call the code that
 * actually does it.
 *
 *      import_statement
 *          = "import" formatted_string "as" SYMBOL
 *
 * @param state
 * @return AstNode*
 *
 */
AstImportStatement* parse_import_statement() {

    ENTER;
    AstImportStatement* node = NULL;
    Token* tok    = get_token();
    AstFormattedString* filename = NULL;
    Token* symbol;

    if(tok->type == TOK_IMPORT) {
        TRACE_TERM(tok);
        finalize_token();
        tok = advance_token();
    }
    else {
        // not an error
        RETV(NULL);
    }

    // NOTE: This calls for a TOK_LITERAL_STR, because the formatted
    // string production has not been written yet.
    // if(tok->type == TOK_LITERAL_STR) {
    //     filename = tok; //copy_string(tok->str);
    //     TRACE("file name = %s", raw_string(tok->str));
    //     finalize_token();
    //     tok = advance_token();
    // }
    if(NULL == (filename = parse_formatted_string())) {
        // show_syntax_error("expected a formatted string, but got a %s",
        //                   tok_to_str(tok->type));
        EXPECTED("a formatted string");
        RETV(NULL);
    }
    tok = get_token();

    if(tok->type == TOK_AS) {
        finalize_token();
        tok = advance_token();
    }
    else {
        // show_syntax_error("expected AS keyword, but got %s", tok_to_str(tok->type));
        EXPECTED("AS keyword");
        RETV(NULL);
    }

    if(tok->type == TOK_SYMBOL) {
        symbol = tok; //copy_string(tok->str);
        TRACE("symbol = \"%s\"", raw_string(tok->str));
        finalize_token();
        // Do not advance token because a new file will be opened and that
        // will do it instead.
        // advance_token();
    }
    else {
        //show_syntax_error("expected a SYMBOL but got a %s", tok_to_str(tok->type));
        EXPECTED("a SYMBOL");
        RETV(NULL);
    }

    node = CREATE_AST_NODE(AST_import_statement, AstImportStatement);
    node->fname = filename;
    node->symbol = symbol;
    finalize_token_queue();

    TRACE("node is complete");
    // handle the actual import
    if(!do_import(filename, symbol->str))
        node = NULL;

    RETV(node);
}
