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
#include "paths.h"

/**
 * @brief Convert mod to an openable file name, Open the file. Parse a module.
 *
 * @param mod
 * @param name
 *
 */
static ast_module* handle_import(Token* mod, Token* name) {

    ENTER;
    ast_module* module;

    // find the file and open it
    open_file(raw_string(find_file(raw_string(mod->str))));

    // push the name
    if(name != NULL)
        push_name(name->str);
    else
        push_name(mod->str);
    push_scope(SCOPE_PRIV);

    // parse the module
    set_import_state(true);
    module = parse_module();
    set_import_state(false);

    // pop the scope and name
    pop_scope();
    pop_name();

    RETV(module);
}

/**
 * @brief The import statement is like a namespace except that it opens a new
 * file and reads from that instead of the current file. When the imported
 * file is finished, then the input stream automatically switches back to the
 * previous file. When an imported file is being read, only the new symbols
 * are placed into the AST so that they can be checked when the symbol resolver
 * pass is entered.
 *
 *  import_statement
 *      = 'import' SYMBOL ('as' SYMBOL)?
 *
 * @return ast_import_statement*
 *
 */
ast_import_statement* parse_import_statement() {

    ENTER;
    ast_import_statement* node = NULL;
    Token* name; // optional alternative name
    Token* mod; // module to import

    if(TOK_IMPORT == TTYPE) {
        advance_token();

        if(TOK_SYMBOL == TTYPE) {
            mod = get_token();
            advance_token();
            if(TOK_AS == TTYPE) {
                advance_token();
                if(TOK_SYMBOL == TTYPE) {
                    name = get_token();
                    advance_token();
                }
                else {
                    EXPECTED(">> a SYMBOL");
                    return NULL;
                }
            }

            node = CREATE_AST_NODE(AST_import_statement, ast_import_statement);
            node->mod = mod;
            node->name = name;
            node->module = handle_import(mod, name);
        }
        else
            EXPECTED("<< a SYMBOL");
    }

    RETV(node);
}
