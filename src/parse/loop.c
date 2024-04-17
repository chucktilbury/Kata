/**
 * @file loop.c
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
 *  while_definition
 *      = 'while' ( '(' ( expression )? ')' )?
 *
 * @return ast_while_definition*
 *
 */
ast_while_definition* parse_while_definition() {

    ENTER;
    ast_while_definition* node = NULL;
    ast_expression* expr = NULL;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // a 'while' token or no match
                if(TOK_WHILE == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // a '(' or completed match
                if(TOK_OPAREN == TTYPE) {
                    advance_token();
                    state = 2;
                }
                else
                    state = 100;
                break;

            case 2:
                // an expression or a ')'
                if(NULL != (expr = parse_expression()))
                    state = 3;
                else if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("an expression or a ')'");
                    state = 102;
                }
                break;

            case 3:
                // a ')' or an error
                if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a ')'");
                    state = 102;
                }
                break;

            case 100:
                // completed parse
                node = CREATE_AST_NODE(AST_while_definition, ast_while_definition);
                // the expr could be NULL
                node->expr = expr;
                finished = true;
                break;

            case 101:
                // not a match
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                node = NULL;
                finished = true;
                break;

            default:
                fatal_error("unhandled state in %s: %d", __func__, state);
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  while_clause
 *      = while_definition function_body
 *
 * @return ast_while_clause*
 *
 */
ast_while_clause* parse_while_clause() {

    ENTER;
    ast_while_clause* node = NULL;
    ast_while_definition* nterm;
    ast_function_body* body;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // while_definition or not a match
                if(NULL != (nterm = parse_while_definition()))
                    state = 1;
                else
                    state = 101;
                break;

            case 1:
                // must be a function body
                if(NULL != (body = parse_function_body()))
                    state = 100;
                else {
                    EXPECTED("a function body");
                    state = 102;
                }
                break;

            case 100:
                // completed parse
                node = CREATE_AST_NODE(AST_while_clause, ast_while_clause);
                node->nterm = nterm;
                node->body = body;
                finished = true;
                break;

            case 101:
                // not a match
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                node = NULL;
                finished = true;
                break;

            default:
                fatal_error("unhandled state in %s: %d", __func__, state);
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  do_clause
 *      = 'do' function_body while_definition
 *
 * @return ast_do_clause*
 *
 */
ast_do_clause* parse_do_clause() {

    ENTER;
    ast_do_clause* node = NULL;
    ast_while_definition* nterm;
    ast_function_body* body;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // 'do' or not a match
                if(TOK_DO == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // must be a function body
                if(NULL != (body = parse_function_body()))
                    state = 2;
                else {
                    EXPECTED("a function body");
                    state = 102;
                }
                break;

            case 2:
                // must be a while definition
                if(NULL != (nterm = parse_while_definition()))
                    state = 100;
                else {
                    EXPECTED("a while keyword");
                    state = 102;
                }
                break;

            case 100:
                // completed parse
                node = CREATE_AST_NODE(AST_do_clause, ast_do_clause);
                node->nterm = nterm;
                node->body = body;
                finished = true;
                break;

            case 101:
                // not a match
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                node = NULL;
                finished = true;
                break;

            default:
                fatal_error("unhandled state in %s: %d", __func__, state);
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  for_clause
 *      = 'for' ( '(' ( (type_name)? SYMBOL 'in' expression )? ')' )?
 *              function_body
 *
 * @return ast_for_clause*
 *
 */
ast_for_clause* parse_for_clause() {

    ENTER;
    ast_for_clause* node = NULL;
    ast_expression* expr = NULL;
    Token* symbol = NULL;
    ast_type_name* type = NULL;
    ast_function_body* body;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // If not a 'for' then not a match
                TRACE("state = %d", state);
                if(TOK_FOR == TTYPE) {
                    state = 1;
                    advance_token();
                }
                else 
                    state = 101;
                break;

            case 1:
                // must be a '(' or a function body or an error
                TRACE("state = %d", state);
                if(TOK_OPAREN == TTYPE) {
                    state = 2;
                    advance_token();
                }
                else if(NULL != (body = parse_function_body())) {
                    state = 100;
                }
                else {
                    EXPECTED("a '(' or a function body");
                    state = 102;
                }
                break;

            case 2:
                // can be a ')', a type name, or a symbol, else error
                TRACE("state = %d", state);
                if(TOK_CPAREN == TTYPE) {
                    state = 7;
                    advance_token();
                }
                else if(TOK_SYMBOL == TTYPE) {
                    state = 4;
                    symbol = get_token();
                    advance_token();
                }
                else if(NULL != (type = parse_type_name())) {
                    state = 3;
                }
                else {
                    EXPECTED("a ')', a type name, or a SYMBOL");
                    state = 102;
                }
                break;

            case 3:
                // must be a SYMBOL or an error
                TRACE("state = %d", state);
                if(TOK_SYMBOL == TTYPE) {
                    state = 4;
                    symbol = get_token();
                    advance_token();
                }
                else {
                    EXPECTED("a SYMBOL");
                    state = 102;
                }
                break;

            case 4:
                // must be an 'in' token or an error
                TRACE("state = %d", state);
                if(TOK_IN == TTYPE) {
                    state = 5;
                    advance_token();
                }
                else {
                    EXPECTED("the 'in' keyword");
                    state = 102;
                }
                break;

            case 5:
                // must be an expression or an error
                TRACE("state = %d", state);
                if(NULL != (expr = parse_expression())) 
                    state = 6;
                else {
                    EXPECTED("an expression");
                    state = 102;
                }
                break;

            case 6:
                // must be a ')' or an error
                TRACE("state = %d", state);
                if(TOK_CPAREN == TTYPE) {
                    state = 7;
                    advance_token();
                }
                else {
                    EXPECTED("a ')'");
                    state = 102;
                }
                break;

            case 7:
                // must be a function body or an error
                TRACE("state = %d", state);
                if(NULL != (body = parse_function_body())) 
                    state = 100;
                else {
                    EXPECTED("a function body");
                    state = 102;
                }
                break;

            case 100:
                // completed parse
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_for_clause, ast_for_clause);
                node->symbol = symbol;
                node->type = type;
                node->expr = expr;
                node->body = body;
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
                node = NULL;
                finished = true;
                break;

            default:
                fatal_error("unhandled state in %s: %d", __func__, state);
        }
    }

    // while(!finished) {
    //     switch(state) {
    //         case 0:
    //             // 'for' or no match
    //             if(TOK_FOR == TTYPE) {
    //                 advance_token();
    //                 state = 1;
    //             }
    //             else
    //                 state = 101;
    //             break;

    //         case 1:
    //             // could be a '(' or a function body
    //             if(TOK_OPAREN == TTYPE) {
    //                 advance_token();
    //                 state = 2;
    //             }
    //             else
    //                 state = 3;
    //             break;

    //         case 2:
    //             // could be a type name, a SYMBOL or a ')', else error
    //             if(TOK_CPAREN == TTYPE) {
    //                 advance_token();
    //                 state = 3;
    //             }
    //             else if(NULL != (type = parse_type_name()))
    //                 state = 5;
    //             else if(TOK_SYMBOL == TTYPE) {
    //                 symbol = get_token();
    //                 advance_token();
    //                 state = 6;
    //             }
    //             break;

    //         case 3:
    //             // must be a function body or error
    //             if(NULL != (body = parse_function_body()))
    //                 state = 100;
    //             else {
    //                 EXPECTED("a function body");
    //                 state = 102;
    //             }
    //             break;

    //         case 4:
    //             // must be a ')' or an error
    //             if(TOK_CPAREN == TTYPE) {
    //                 advance_token();
    //                 state = 3;
    //             }
    //             else {
    //                 EXPECTED("a ')'");
    //                 state = 102;
    //             }
    //             break;

    //         case 5:
    //             // must be a symbol or an error
    //             if(TOK_SYMBOL == TTYPE) {
    //                 symbol = get_token();
    //                 advance_token();
    //                 state = 6;
    //             }
    //             else {
    //                 EXPECTED("a SYMBOL");
    //                 state = 102;
    //             }
    //             break;

    //         case 6:
    //             // must be an 'in' keyword or error
    //             if(TOK_IN == TTYPE) {
    //                 advance_token();
    //                 state = 7;
    //             }
    //             else {
    //                 EXPECTED("the 'in' keyword");
    //                 state = 102;
    //             }
    //             break;

    //         case 7:
    //             // must be an expression or error
    //             if(NULL != (expr = parse_expression()))
    //                 state = 4;
    //             else {
    //                 EXPECTED("an expression");
    //                 state = 102;
    //             }
    //             break;

    //         case 100:
    //             // completed parse
    //             node = CREATE_AST_NODE(AST_for_clause, ast_for_clause);
    //             node->symbol = symbol;
    //             node->type = type;
    //             node->expr = expr;
    //             node->body = body;
    //             finished = true;
    //             break;

    //         case 101:
    //             // not a match
    //             reset_token_queue(post);
    //             finished = true;
    //             break;

    //         case 102:
    //             // error
    //             node = NULL;
    //             finished = true;
    //             break;

    //         default:
    //             fatal_error("unhandled state in %s: %d", __func__, state);
    //     }
    // }

    RETV(node);
}


