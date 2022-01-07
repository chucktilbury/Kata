/*
 * Parser main interface.
 *
 * When this is called, there should already be a file open for the scanner.
 * A AST data object is returned upon success, or NULL upon failure.
 */
#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "parser.h"
#include "token.h"
#include "emit.h"
#include "symbols.h"

/*
 * Retrieve a name space. A namespace can have anything a module can have,
 * except an include statement. Namespaces can be nested.
 *
 * A namespace does not really emit any C code. The namespace is used to
 * decorate names so that they are unique to that namespace.
 *
 * format:
 * namespace SYMBOL { statements }
 */
static void parse_namespace() {

    token* tok = scan_token();
    if(tok->type == SYMBOL_TOK) {
        LOG(1, "namespace symbol: %s", tok->str);
        symbol sym = create_symbol(tok->str);
        set_symbol_attr(sym, "type", &tok->type, sizeof(tok->type));
    }
    else {
        syntax("expected symbol but got a %s", tok_to_str(tok->type));
        destroy_token(tok);
        recover_from_error();
        return;
    }
}

/*
 * Retrieve a class with the optional single inheritance. A class can have data
 * declarations and function declarations. The create() and destroy() ctor and
 * dtor definitions are optional. The user defined ctor and dtor are executed
 * after the default ones.
 *
 * format:
 * class SYMBOL { [data_decl|function_decl]* }
 * class SYMBOL ( COMPOUND_SYMBOL ) { [data_decl|function_decl]* }
 */
static void parse_class() {
}

/*
 * An include statement switches the input stream to the file included and
 * when that file is finished, the stream picks up where it left off.
 *
 * Includes in Simple are used to introduce external libraries and allow access
 * to their symbols in this module.
 *
 * format:
 * import STRING
 */
static void parse_import() {
}

/*
 * A function definition has the following form:
 * type classname.funcname ( parameter list ) block.
 *
 * Function names are decorated according to the namespace, class name, and
 * parameter names to make them unique.
 *
 * format:
 * TYPE COMPOUND_SYMBOL ( parameter_list ) { func_statements }
 */
static void parse_function_def(token* tok) {

    destroy_token(tok);
}

/*
 * A module can have a namespace, a class, or a function definition in any
 * order and in any quantity.
 */
static void parse_module() {

    int finished = 0;

    while(!finished) {
        token* tok = scan_token();
        switch(tok->type) {
            case NAMESPACE_TOK:
                parse_namespace();
                destroy_token(tok);
                break;
            case CLASS_TOK:
                parse_class();
                destroy_token(tok);
                break;
            case IMPORT_TOK:
                parse_import();
                destroy_token(tok);
                break;
            case FLOAT_TOK:
            case INT_TOK:
            case UINT_TOK:
            case BOOL_TOK:
            case DICT_TOK:
            case LIST_TOK:
            case STRING_TOK:
                parse_function_def(tok);
                break;
            case INPUT_END_TOK:
                finished++;
                destroy_token(tok);
                break;
            default:
                syntax("expected include specification, namespace, class, or function definition, but got %s", tok_to_str(tok->type));
                destroy_token(tok);
                recover_from_error();
        }
    }
}

/*
 * Public interface.
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
}
