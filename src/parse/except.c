/**
 * @file except.c
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
 *  try_clause
 *      = 'try' function_body ( except_clause )* ( final_clause )?
 *
 * @return ast_try_clause*
 *
 */
ast_try_clause* parse_try_clause() {

    ENTER;
    ast_try_clause* node = NULL;
    ast_function_body* fbod;
    ast_final_clause* final = NULL;
    LinkList* except_list = create_link_list();
    ast_except_clause* ecla;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // try or not a match
                TRACE("state = %d", state);
                if(TOK_TRY == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // function body or error
                TRACE("state = %d", state);
                if(NULL == (fbod = parse_function_body())) {
                    EXPECTED("a function body");
                    state = 102;
                }
                else 
                    state = 2;
                break;

            case 2:
                // except clause or final clause
                TRACE("state = %d", state);
                if(NULL != (final = parse_final_clause())) 
                    state = 100;
                else if(NULL != (ecla = parse_except_clause()))
                    append_link_list(except_list, ecla); // keep the same state
                else {
                    // anything else finishes the production
                    state = 100;
                }
                break;

            case 100:
                // create node and return
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_try_clause, ast_try_clause);
                node->fbod = fbod;
                node->except_list = except_list;
                node->final = final;
                finished = true;
                break;

            case 101:
                // not a match
                TRACE("state = %d", state);
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                TRACE("state = %d", state);
                finished = true;
                break;

            default:
                TRACE("state = %d", state);
                fatal_error("unknown state in %s (%d)", __func__, state);
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  except_clause_mid
 *      = 'except' '(' SYMBOL ',' SYMBOL ')' function_body
 *
 * @return ast_except_clause_mid*
 *
 */
ast_except_clause* parse_except_clause() {

    ENTER;
    ast_except_clause* node = NULL;
    Token* name;
    Token* msg;
    ast_function_body* fbod;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();


    while(!finished) {
        switch(state) {
            case 0:
                // try or not a match
                TRACE("state = %d", state);
                if(TOK_EXCEPT == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // open paren or error
                TRACE("state = %d", state);
                if(TOK_OPAREN == TTYPE) {
                    advance_token();
                    state = 2;
                }
                else {
                    EXPECTED("a '('");
                    state = 102;
                }
                break;

            case 2:
                // symbol or error
                TRACE("state = %d", state);
                if(TOK_SYMBOL == TTYPE) {
                    TRACE_TERM(get_token());
                    advance_token();
                    name = get_token();
                    state = 3;
                }
                else {
                    EXPECTED("a SYMBOL");
                    state = 102;
                }
                break;

            case 3:
                // comma or error
                TRACE("state = %d", state);
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    state = 4;
                }
                else {
                    EXPECTED("a ','");
                    state = 102;
                }
                break;

            case 4:
                // symbol or error
                TRACE("state = %d", state);
                if(TOK_SYMBOL == TTYPE) {
                    TRACE_TERM(get_token());
                    advance_token();
                    msg = get_token();
                    state = 5;
                }
                else {
                    EXPECTED("a SYMBOL");
                    state = 102;
                }
                break;

            case 5:
                // close paren or error
                TRACE("state = %d", state);
                if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    state = 6;
                }
                else {
                    EXPECTED("a ')'");
                    state = 102;
                }
                break;

            case 6:
                // function body or error
                TRACE("state = %d", state);
                if(NULL != (fbod = parse_function_body()))
                    state = 100;
                else {
                    EXPECTED("a function body");
                    state = 102;
                }
                break;

            case 100:
                // create node and return
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_except_clause, ast_except_clause);
                node->fbod = fbod;
                node->msg = msg;
                node->name = name;
                finished = true;
                break;

            case 101:
                // not a match
                TRACE("state = %d", state);
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                TRACE("state = %d", state);
                finished = true;
                break;

            default:
                TRACE("state = %d", state);
                fatal_error("unknown state in %s (%d)", __func__, state);
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  final_clause
 *      = 'final' '(' SYMBOL ')' function_body
 *
 * @return ast_final_clause*
 *
 */
ast_final_clause* parse_final_clause() {

    ENTER;
    ast_final_clause* node = NULL;
    Token* msg;
    ast_function_body* fbod;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();


    while(!finished) {
        switch(state) {
            case 0:
                // try or not a match
                TRACE("state = %d", state);
                if(TOK_FINAL == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // open paren or error
                TRACE("state = %d", state);
                if(TOK_OPAREN == TTYPE) {
                    advance_token();
                    state = 2;
                }
                else {
                    EXPECTED("a '('");
                    state = 102;
                }
                break;

            case 2:
                // symbol or error
                TRACE("state = %d", state);
                if(TOK_SYMBOL == TTYPE) {
                    TRACE_TERM(get_token());
                    advance_token();
                    msg = get_token();
                    state = 3;
                }
                else {
                    EXPECTED("a SYMBOL");
                    state = 102;
                }
                break;

            case 3:
                // close paren or error
                TRACE("state = %d", state);
                if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    state = 4;
                }
                else {
                    EXPECTED("a ')'");
                    state = 102;
                }
                break;

            case 4:
                // function body or error
                TRACE("state = %d", state);
                if(NULL != (fbod = parse_function_body()))
                    state = 100;
                else {
                    EXPECTED("a function body");
                    state = 102;
                }
                break;

            case 100:
                // create node and return
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_final_clause, ast_final_clause);
                node->fbod = fbod;
                node->msg = msg;
                finished = true;
                break;

            case 101:
                // not a match
                TRACE("state = %d", state);
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                TRACE("state = %d", state);
                finished = true;
                break;

            default:
                TRACE("state = %d", state);
                fatal_error("unknown state in %s (%d)", __func__, state);
        }
    }

    RETV(node);
}

