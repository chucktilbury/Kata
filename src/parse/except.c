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
#define USE_TRACE 1
//#include "strings.h"
#include "parse.h"
#include "scanner.h"

/**
 * @brief
 *
 *  try_clause
 *      = 'try' function_body except_clause
 *
 * @return ast_try_clause*
 *
 */
ast_try_clause* parse_try_clause() {

    ENTER;
    ast_try_clause* node = NULL;
    ast_function_body* fbod;
    ast_except_clause* ecla;

    if(TOK_TRY == TTYPE) {
        advance_token();

        if(NULL != (fbod = parse_function_body())) {

            if(NULL != (ecla = parse_except_clause())) {
                node = CREATE_AST_NODE(AST_try_clause, ast_try_clause);
                node->fbod = fbod;
                node->ecla = ecla;
            }
            else {
                EXPECTED("an exception handler");
            }
        }
        else {
            EXPECTED("a function body");
        }
    }
    // else no tokes consumed

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
ast_except_clause_mid* parse_except_clause_mid() {

    ENTER;
    ast_except_clause_mid* node = NULL;
    Token* name;
    Token* msg;
    ast_function_body* fbod;
    void* post = post_token_queue();

    if(TOK_EXCEPT == TTYPE) {
        advance_token();
        if(TOK_OPAREN == TTYPE) {
            advance_token();
            if(TOK_SYMBOL == TTYPE) {
                name = get_token();
                advance_token();
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    if(TOK_SYMBOL == TTYPE) {
                        msg = get_token();
                        advance_token();
                        if(TOK_CPAREN == TTYPE) {
                            advance_token();
                            if(NULL != (fbod = parse_function_body())) {
                                node = CREATE_AST_NODE(AST_except_clause_mid, ast_except_clause_mid);
                                node->name = name;
                                node->msg = msg;
                                node->fbod = fbod;
                            }
                            else
                                EXPECTED("a function body");
                        }
                        else
                            EXPECTED("a ')'");
                    }
                    else
                        EXPECTED("a valid symbol");
                }
                else
                    EXPECTED("a ','");
            }
            else
                reset_token_queue(post);
        }
        else
            EXPECTED("a '('");
    }
    // else no tokens consumed

    RETV(node);
}

/**
 * @brief
 *
 *  except_clause_final
 *      = 'except' '(' 'any' ',' SYMBOL ')' function_body
 *
 * @return ast_except_clause_final*
 *
 */
ast_except_clause_final* parse_except_clause_final() {

    ENTER;
    ast_except_clause_final* node = NULL;
    Token* msg;
    ast_function_body* fbod;
    void* post = post_token_queue();

    if(TOK_EXCEPT == TTYPE) {
        advance_token();
        if(TOK_OPAREN == TTYPE) {
            advance_token();
            if(TOK_ANY == TTYPE) {
                advance_token();
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    if(TOK_SYMBOL == TTYPE) {
                        msg = get_token();
                        advance_token();
                        if(TOK_CPAREN == TTYPE) {
                            advance_token();
                            if(NULL != (fbod = parse_function_body())) {
                                node = CREATE_AST_NODE(AST_except_clause_final, ast_except_clause_final);
                                node->msg = msg;
                                node->fbod = fbod;
                            }
                            else
                                EXPECTED("a function body");
                        }
                        else
                            EXPECTED("a ')'");
                    }
                    else
                        EXPECTED("a valid symbol");
                }
                else
                    EXPECTED("a ','");
            }
            else
                reset_token_queue(post);
        }
        else
            EXPECTED("a '('");
    }
    // else no tokens consumed

    RETV(node);
}

/**
 * @brief
 *
 *  except_clause
 *      = ( except_clause_mid )+ ( except_clause_final )?
 *      / except_clause_final
 *
 * @return ast_except_clause*
 *
 */
ast_except_clause* parse_except_clause() {

    ENTER;
    ast_except_clause* node = NULL;
    LinkList* list = create_link_list();
    ast_except_clause_mid* mid = NULL;
    ast_except_clause_final* fin = NULL;

    int state = 0;
    bool finished = false;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // initial state
                if(NULL != (mid = parse_except_clause_mid())) {
                    append_link_list(list, mid);
                    state = 1;
                }
                else if(NULL != (fin = parse_except_clause_final())) {
                    state = 100;
                }
                else
                    state = 101;
                break;

            case 1:
                // repeat for mid clause
                if(NULL != (mid = parse_except_clause_mid())) {
                    append_link_list(list, mid);
                    state = 1;
                }
                else if(NULL != (fin = parse_except_clause_final())) {
                    state = 100;
                }
                else
                    EXPECTED("an exception handler");
                break;

            case 100:
                // successful parse
                node = CREATE_AST_NODE(AST_except_clause, ast_except_clause);
                node->fin = fin;
                node->list = list;
                finalize_token_queue();
                finished = true;
                break;

            case 101:
                // successful parse of final only
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                finished = true;
                break;

            default:
                fatal_error("unexpected state in %s: %d", __func__, state);

        }
    }

    RETV(node);
}


