/**
 * @file compound.c
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
 * @brief
 *
 *  compound_name
 *      = SYMBOL ( '.' SYMBOL )*
 *
 * @return ast_compound_name*
 *
 */
ast_compound_name* parse_compound_name() {

    ENTER;
    ast_compound_name* node = NULL;
    LList lst = create_llist();
    Str* str = create_string(NULL);
    Token* tok;
    void* post = post_token_queue();

    bool finished = false;
    int state = 0;

    while(!finished) {
        tok = get_token();
        switch(state) {
            case 0:
                // initial state
                if(TOK_SYMBOL == token_type(tok)) {
                    append_llist(lst, tok);
                    add_string_Str(str, tok->str);
                    TRACE_TERM(tok);
                    advance_token();
                    state = 1;
                }
                else {
                    // not a symbol, not an error
                    state = 101;
                }
                break;
            case 1:
                // seen a symbol, this must be a dot or something else.
                if(TOK_DOT == token_type(tok)) {
                    add_string_char(str, '.');
                    advance_token();
                    state = 2;
                }
                else {
                    state = 100;
                }
                break;
            case 2:
                // must be a symbol or it's not a match (conflict with create/destroy)
                if((TOK_CREATE == TTYPE) || (TOK_DESTROY == TTYPE))
                    state = 101;
                else if(TOK_SYMBOL == token_type(tok)) {
                    TRACE_TERM(tok);
                    append_llist(lst, tok);
                    add_string_Str(str, tok->str);
                    advance_token();
                    state = 1;
                }
                else {
                    state = 102;
                }
                break;
            case 100:
                // is a compound name, and not an error
                node = CREATE_AST_NODE(AST_compound_name, ast_compound_name);
                node->list = lst;
                node->raw_name = str;
                finalize_token_queue();
                finished = true;
                break;
            case 101:
                // not a compound symbol, not an error
                reset_token_queue(post);
                finished = true;
                break;
            case 102:
                // not a compound name, and is an error
                EXPECTED("a SYMBOL os a '.'");
                finished = true;
                node = NULL;
                break;
            default:
                fatal_error("unknown state number in %s: %d", __func__, state);
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  compound_name_list
 *      = '(' ( compound_name (',' compound_name)* )? ')'
 *
 * @return ast_compound_name_list*
 *
 */
ast_compound_name_list* parse_compound_name_list() {

    ENTER;
    ast_compound_name_list* node = NULL;
    ast_compound_name* nterm;
    LList list = create_llist();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // must start with a '(' or not a list
                if(TOK_OPAREN == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // must be a compound name or a ')'
                if(NULL != (nterm = parse_compound_name())) {
                    append_llist(list, nterm);
                    state = 2;
                }
                else if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a compound name or a ')'");
                    state = 102;
                }
                break;

            case 2:
                // must be a ',' or a ')'
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    state = 3;
                }
                if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a ',' or a ')'");
                    state = 102;
                }
                break;

            case 3:
                // must be a compound name or it's an error
                if(NULL != (nterm = parse_compound_name())) {
                    append_llist(list, nterm);
                    state = 2;
                }
                else {
                    EXPECTED("a compound name");
                    state = 102;
                }
                break;

            case 100:
                // complete non-terminal parsed
                node = CREATE_AST_NODE(AST_compound_name_list, ast_compound_name_list);
                node->list = list;
                finished = true;
                break;

            case 101:
                // not a match, not an error
                finished = true;
                break;

            case 102:
                // is an error.
                node = NULL;
                finished = true;
                break;
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  compound_ref_item
 *      = SYMBOL
 *      / array_reference
 *
 * @return ast_compound_ref_item*
 *
 */
ast_compound_ref_item* parse_compound_ref_item() {

    ENTER;
    ast_compound_ref_item* node = NULL;
    ast_array_reference* nterm;
    Token* tok = get_token();
    void* post = post_token_queue();

    if(TOK_SYMBOL == token_type(tok)) {
        node = CREATE_AST_NODE(AST_compound_ref_item, ast_compound_ref_item);
        TRACE_TERM(tok);
        node->token = tok;
        node->nterm = NULL;
        advance_token();
    }
    else if(NULL != (nterm = parse_array_reference())) {
        node = CREATE_AST_NODE(AST_compound_ref_item, ast_compound_ref_item);
        node->token = NULL;
        node->nterm = (ast_node*)nterm;
        finalize_token_queue();
    }
    else {
        reset_token_queue(post);
    }

    RETV(node);
}

/**
 * @brief
 *
 *  compound_reference
 *      = compound_ref_item ( '.' compound_ref_item )*
 *
 * @return ast_compound_reference*
 *
 */
ast_compound_reference* parse_compound_reference() {

    ENTER;
    ast_compound_reference* node = NULL;
    ast_compound_ref_item* nterm;
    LList list = create_llist();
    Token* tok;
    int s = 0;
    bool finished = false;
    void* post = post_token_queue();

    while(!finished) {
        tok = get_token();
        switch(s) {
            case 0:
                // entry point
                if(NULL != (nterm = parse_compound_ref_item())) {
                    append_llist(list, nterm);
                    s = 1;
                }
                else {
                    s = 101;
                }
                break;
            case 1:
                // optional dot. if it's not a dot, then return the nterm
                if(TOK_DOT == token_type(tok)) {
                    advance_token();
                    s = 2;
                }
                else {
                    s = 100;
                }
                break;
            case 2:
                // must be a compound_ref_item
                if(NULL != (nterm = parse_compound_ref_item())) {
                    append_llist(list, nterm);
                    s = 1;
                }
                else {
                    s = 102;
                }
                break;
            case 100:
                // finished, not an error, return the node
                node = CREATE_AST_NODE(AST_compound_reference, ast_compound_reference);
                node->list = list;
                finalize_token_queue();
                finished = true;
                break;
            case 101:
                // finished, not a node, not an error
                reset_token_queue(post);
                finished = true;
                break;
            case 102:
                // finished, is an error
                EXPECTED("a SYMBOL or a '.'");
                node = NULL;
                finished = true;
                break;
            default:
                fatal_error("unknown state number in %s: %d", __func__, s);

        }
    }

    RETV(node);
}


