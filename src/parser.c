/*
 * Parser main interface.
 *
 * When this is called, there should already be a file open for the scanner.
 * A AST data object is returned upon success, or NULL upon failure.
 */
#include "common.h"


/*
 * This is the top level entry point of the parser. When this is called, no
 * tokens from the current file (AKA module) have been read yet.
 *
 * The object that is encountered at the top level must a definition of one of
 * the following:
 *  class
 *  import
 *  function
 * These may appear in any number and in any order. Note that data definitions
 * must be in a class.
 *
 */
static void parse_module() {

    int finished = 0;

    while(!finished) {
        token* tok = get_token();
        switch(tok->type) {
            // The protected scope makes no sense here.
            case PUBLIC_TOK:
            case PRIVATE_TOK:
            case CLASS_TOK:
                parse_class();
                break;
            case IMPORT_TOK:
                parse_import();
                break;
            case COMPOUND_NAME_TOK:
                parse_function_def(tok);
                break;
            case INPUT_END_TOK:
                finished++;
                break;
            default:
                syntax("expected include specification, class, or function definition, but got %s", tok_to_str(tok->type));
        }
    }
}

/*
 * Public entry point of the parser. A file must have already been opened in
 * the scanner.
 */
int parse() {

    emit_preamble();
    parse_module();
    emit_postamble();

    return 0;
}

/*
 * Read and throw away tokens until we come to a point where parsing can
 * start again.
 */
void recover_from_error() {

    bool finished = false;
    token* tok = next_token();
    ENTER;

    // eat tokens until we are at a good stopping place.
    do {
        if(tok->type == CCURLY_TOK) {
            next_token();
            finished = true;
        }
        else
            next_token();

    } while(!finished && tok->type != INPUT_END_TOK);

    LEAVE;
}

/*
 * Create the parser and open the initial file.
 */
void init_parser(const char* fname) {

    init_scanner();
    scanner_open(fname);

    next_token();   // start the token stream.
}
