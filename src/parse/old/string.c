/**
 * @file string.c
 *
 * @brief Parse a formatted string. The expression list is optional.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-20-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_parser.h"

AstFormattedString* parse_formatted_string() {

    ENTER;
    Token* tok = get_token();
    AstFormattedString* node = NULL;

    if(TOK_LITERAL_DSTR == tok->type) {
        node = CREATE_AST_NODE(AST_formatted_string, AstFormattedString);
        node->strg = tok;
        TRACE_TERM(tok);
        finalize_token();
        advance_token();

        // expression list is optional.
        node->expr_lst = parse_expression_list();

        // AstExpressionList* nterm = NULL;
        // if(NULL != (nterm = parse_expression_list()))
        //     node->expr_lst = nterm;
        // else {
        //     show_syntax_error("error parsing expression list");
        //     RETV(NULL);
        // }
    }

    // NULL return is not an error
    RETV(node);
}