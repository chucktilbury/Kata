/**
 * @file flow.c
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
 *  if_clause
 *      = 'if' '(' expression ')' function_body ( elif_clause )* ( else_clause )?
 *
 * @return ast_if_clause*
 *
 */
ast_if_clause* parse_if_clause() {

    ENTER;
    ast_if_clause* node = NULL;
    ast_expression* expr;
    ast_function_body* fbod;
    ast_else_clause* eclaus = NULL;
    ast_elif_clause* elif = NULL;
    LinkList* elif_list = create_link_list();


    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // if or not a match
                TRACE("state = %d", state);
                if(TOK_IF == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // oparen is required or error
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
                // expression is required or error
                TRACE("state = %d", state);
                if(NULL == (expr = parse_expression())) {
                    EXPECTED("an expression");
                    state = 102;
                }
                else 
                    state = 3;
                break;

            case 3:
                // expect a ')' or error
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
                // expect a function body or error
                TRACE("state = %d", state);
                if(NULL == (fbod = parse_function_body())) {
                    EXPECTED("a function body");
                    state = 102;
                }
                else 
                    state = 5;
                break;

            case 5:
                // expect an elif or an else, otherwise finished
                TRACE("state = %d", state);
                if(NULL != (eclaus = parse_else_clause())) 
                    state = 100;
                else if(NULL != (elif = parse_elif_clause())) {
                    append_link_list(elif_list, elif);
                    // keep this state
                }
                else 
                    state = 100;
                break;

            case 100:
                // create node and return
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_if_clause, ast_if_clause);
                node->expr = expr;
                node->fbod = fbod;
                node->eclaus = eclaus;
                node->elif_list = elif_list;
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
 *  elif_clause
 *      = 'elif' '(' expression ')' function_body
 *
 * @return ast_elif_clause*
 *
 */
ast_elif_clause* parse_elif_clause() {

    ENTER;
    ast_elif_clause* node = NULL;
    ast_expression* expr;
    ast_function_body* fbod;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // elif or not a match
                TRACE("state = %d", state);
                if(TOK_ELIF == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // oparen is required or error
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
                // expression is required or error
                TRACE("state = %d", state);
                if(NULL == (expr = parse_expression())) {
                    EXPECTED("an expression");
                    state = 102;
                }
                else 
                    state = 3;
                break;

            case 3:
                // expect a ')' or error
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
                // expect a function body or error
                TRACE("state = %d", state);
                if(NULL == (fbod = parse_function_body())) {
                    EXPECTED("a function body");
                    state = 102;
                }
                else 
                    state = 100;
                break;

            case 100:
                // create node and return
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_elif_clause, ast_elif_clause);
                node->expr = expr;
                node->fbod = fbod;
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
 *  else_clause
 *      = 'else' function_body
 *
 * @return ast_else_clause*
 *
 */
ast_else_clause* parse_else_clause() {

    ENTER;
    ast_else_clause* node = NULL;
    ast_function_body* fbod;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // elif or not a match
                TRACE("state = %d", state);
                if(TOK_ELSE == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // expect a function body or error
                TRACE("state = %d", state);
                if(NULL == (fbod = parse_function_body())) {
                    EXPECTED("a function body");
                    state = 102;
                }
                else 
                    state = 100;
                break;

            case 100:
                // create node and return
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_else_clause, ast_else_clause);
                node->fbod = fbod;
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

