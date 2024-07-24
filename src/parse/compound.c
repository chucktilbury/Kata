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
#include "trace.h"
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
ast_compound_name* parse_compound_name(void) {

    ENTER;
    ast_compound_name* node = NULL;
    LinkList* lst = create_link_list();
    String* str = create_string(NULL);
    Token* tok;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        tok = get_token();
        switch(state) {
            case 0:
                // initial state
                TRACE("state = %d", state);
                if(TOK_SYMBOL == token_type(tok)) {
                    append_link_list(lst, tok);
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
                TRACE("state = %d", state);
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
                TRACE("state = %d", state);
                if((TOK_CREATE == TTYPE) || (TOK_DESTROY == TTYPE))
                    state = 101;
                else if(TOK_SYMBOL == token_type(tok)) {
                    TRACE_TERM(tok);
                    append_link_list(lst, tok);
                    add_string_Str(str, tok->str);
                    advance_token();
                    state = 1;
                }
                else {
                    state = 102;
                    EXPECTED("a SYMBOL");
                }
                break;

            case 100:
                // is a compound name, and not an error
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_compound_name, ast_compound_name);
                node->list = lst;
                node->raw_name = str;
                finished = true;
                break;

            case 101:
                // not a compound symbol, not an error
                TRACE("state = %d", state);
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // not a compound name, and is an error
                TRACE("state = %d", state);
                finished = true;
                node = NULL;
                break;

            default:
                TRACE("state = %d", state);
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
ast_compound_name_list* parse_compound_name_list(void) {

    ENTER;
    ast_compound_name_list* node = NULL;
    ast_compound_name* nterm;
    LinkList* list = create_link_list();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // must start with a '(' or not a list
                TRACE("state = %d", state);
                TRACE_TERM(get_token());
                if(TOK_OPAREN == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // must be a compound name or a ')'
                TRACE("state = %d", state);
                TRACE_TERM(get_token());
                if(NULL != (nterm = parse_compound_name())) {
                    append_link_list(list, nterm);
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
                // must be a ',' or a ')' or an error
                TRACE("state = %d", state);
                TRACE_TERM(get_token());
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    state = 3;
                }
                else if(TOK_CPAREN == TTYPE) {
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
                TRACE("state = %d", state);
                TRACE_TERM(get_token());
                if(NULL != (nterm = parse_compound_name())) {
                    append_link_list(list, nterm);
                    state = 2;
                }
                else {
                    EXPECTED("a compound name");
                    state = 102;
                }
                break;

            case 100:
                // complete non-terminal parsed
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_compound_name_list, ast_compound_name_list);
                node->list = list;
                finished = true;
                break;

            case 101:
                // not a match, not an error
                TRACE("state = %d", state);
                finished = true;
                break;

            case 102:
                // is an error.
                TRACE("state = %d", state);
                node = NULL;
                finished = true;
                break;

            default:
                TRACE("state = %d", state);
                fatal_error("unknown state number in %s: %d", __func__, state);
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  compound_ref_item
 *      = SYMBOL ( array_param_list )?
 *
 * @return ast_compound_ref_item*
 *
 */
ast_compound_ref_item* parse_compound_ref_item(void) {

    ENTER;
    ast_compound_ref_item* node = NULL;
    ast_array_param_list* apar = NULL;
    Token* symb = NULL;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // must be a name or not a match
                TRACE("state = %d", state);
                TRACE_TERM(get_token());
                if(TOK_SYMBOL == TTYPE) {
                    symb = get_token();
                    advance_token();
                    state = 1;
                }
                else 
                    state = 101;
                break;

            case 1:
                // look ahead to see if there is a '['
                TRACE("state = %d", state);
                if(TOK_OSBRACE == TTYPE) {
                    // do not consume the token; needed by array param list
                    if(NULL == (apar = parse_array_param_list()))
                        state = 102;
                    else 
                        state = 100;
                }
                else 
                    state = 100;
                break;

            case 100:
                // complete non-terminal parsed
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_compound_ref_item, ast_compound_ref_item);
                node->apar = apar;
                node->symb = symb; 
                finished = true;
                break;

            case 101:
                // not a match, not an error
                TRACE("state = %d", state);
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // is an error.
                TRACE("state = %d", state);
                node = NULL;
                finished = true;
                break;

            default:
                TRACE("state = %d", state);
                fatal_error("unknown state number in %s: %d", __func__, state);
        }
    }

    // if(TOK_SYMBOL == token_type(tok)) {
    //     TRACE_TERM(get_token());
    //     node = CREATE_AST_NODE(AST_compound_ref_item, ast_compound_ref_item);
    //     TRACE_TERM(tok);
    //     node->token = tok;
    //     node->nterm = NULL;
    //     advance_token();
    // }
    // else if(NULL != (nterm = parse_array_reference())) {
    //     TRACE("array reference");
    //     node = CREATE_AST_NODE(AST_compound_ref_item, ast_compound_ref_item);
    //     node->token = NULL;
    //     node->nterm = (ast_node*)nterm;
    // }
    // else {
    //     reset_token_queue(post);
    // }

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
ast_compound_reference* parse_compound_reference(void) {

    ENTER;
    ast_compound_reference* node = NULL;
    ast_compound_ref_item* nterm;
    LinkList* list = create_link_list();
    Token* tok;

    int state = 0;
    bool finished = false;
    void* post = post_token_queue();

    while(!finished) {
        tok = get_token();
        switch(state) {
            case 0:
                // entry point
                TRACE("state = %d", state);
                if(NULL != (nterm = parse_compound_ref_item())) {
                    append_link_list(list, nterm);
                    state = 1;
                }
                else {
                    state = 101;
                }
                break;

            case 1:
                // optional dot. if it's not a dot, then return the nterm
                TRACE("state = %d", state);
                if(TOK_DOT == token_type(tok)) {
                    advance_token();
                    state = 2;
                }
                else {
                    state = 100;
                }
                break;

            case 2:
                // must be a compound_ref_item
                TRACE("state = %d", state);
                if(NULL != (nterm = parse_compound_ref_item())) {
                    append_link_list(list, nterm);
                    state = 1;
                }
                else {
                    EXPECTED("a SYMBOL or a '.'");
                    state = 102;
                }
                break;

            case 100:
                // finished, not an error, return the node
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_compound_reference, ast_compound_reference);
                node->list = list;
                //finalize_token_queue();
                finished = true;
                break;

            case 101:
                // finished, not a node, not an error
                TRACE("state = %d", state);
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // finished, is an error
                TRACE("state = %d", state);
                node = NULL;
                finished = true;
                break;

            default:
                TRACE("state = %d", state);
                fatal_error("unknown state number in %s: %d", __func__, state);

        }
    }

    RETV(node);
}


