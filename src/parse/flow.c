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
#define USE_TRACE 1
#include "util.h"
#include "parse.h"
#include "scanner.h"

/**
 * @brief
 *
 *  if_clause
 *      = 'if' '(' expression ')' function_body else_clause
 *
 * @return ast_if_clause*
 *
 */
ast_if_clause* parse_if_clause() {

    ENTER;
    ast_if_clause* node = NULL;
    ast_expression* expr;
    ast_function_body* fbod;
    ast_else_clause* els;

    if(TOK_IF == TTYPE) {
        advance_token();
        if(TOK_OPAREN == TTYPE) {
            advance_token();
            if(NULL != (expr = parse_expression())) {
                if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    if(NULL != (fbod = parse_function_body())) {
                        els = parse_else_clause(); // optional
                        node = CREATE_AST_NODE(AST_if_clause, ast_if_clause);
                        node->fbod = fbod;
                        node->eclaus = els;
                        node->expr = expr;
                    }
                    else
                        EXPECTED("a function body");
                }
                else
                    EXPECTED("a ')'");
            }
            else
                EXPECTED("an expression");
        }
        else
            EXPECTED("a '('");

    }
    // else not an if clause

    RETV(node);
}

/**
 * @brief These are combined because they are so similar.
 *
 *  else_clause_mid
 *      = 'else' '(' expression ')' function_body
 *  else_clause_final
 *      = 'else' ( '(' ')' )? function_body
 *
 * @return ast_else_clause_item*
 *
 */
ast_else_clause_item* parse_else_clause_item() {

    ENTER;
    ast_else_clause_item* node = NULL;
    ast_expression* expr;
    ast_function_body* fbod;

    if(TOK_ELSE == TTYPE) {
        advance_token();
        if(TOK_OPAREN == TTYPE) {
            advance_token();
            if(NULL != (expr = parse_expression())) {
                if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    if(NULL != (fbod = parse_function_body())) {
                        node = CREATE_AST_NODE(AST_else_clause_mid, ast_else_clause_item);
                        node->fbod = fbod;
                        node->expr = expr;
                    }
                    else
                        EXPECTED("a function body");
                }
                else
                    EXPECTED("a ')'");
            }
            else {
                // have parens but no expression means we are parsing a else-final
                if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    if(NULL != (fbod = parse_function_body())) {
                        node = CREATE_AST_NODE(AST_else_clause_final, ast_else_clause_item);
                        node->fbod = fbod;
                    }
                    else
                        EXPECTED("a function body");
                }
                else
                    EXPECTED("a ')'");
            }

        }
        else {
            // have no parens means parsing a else-final
            if(NULL != (fbod = parse_function_body())) {
                node = CREATE_AST_NODE(AST_else_clause_final, ast_else_clause_item);
                node->fbod = fbod;
            }
            else
                EXPECTED("a function body");
        }
    }
    // else not an else clause

    RETV(node);
}

/**
 * @brief The description below gives the node type, not the data type. The
 * data type is combined. See above. If an else appears after a "final" else,
 * then it will be picked up as a syntax error my the next rule that is
 * parsed.
 *
 *  else_clause
 *      = ( ( else_clause_mid )* ( else_clause_final )? )?
 *
 * @return ast_else_clause*
 *
 */
ast_else_clause* parse_else_clause() {

    ENTER;
    ast_else_clause* node = NULL;
    ast_else_clause_item* nterm;
    LList list = create_llist();
    void* post = post_token_queue();

    bool finished = false;
    int state = 0;

    while(!finished) {
        switch(state) {
            case 0:
                // initial needs to be absent, or a mid, or a final, else none
                if(NULL == (nterm = parse_else_clause_item())) {
                    if(AST_else_clause_mid == ast_node_type(nterm)) {
                        append_llist(list, nterm);
                        state = 1;
                    }
                    else if(AST_else_clause_final == ast_node_type(nterm)){
                        append_llist(list, nterm);
                        state = 100;
                    }
                    else
                        state = 101; // should never happen
                }
                else
                    // not an else clause
                    state = 101;
                break;

            case 1:
                // initial needs to be absent, or a mid, or a final, else one or more
                if(NULL == (nterm = parse_else_clause_item())) {
                    if(AST_else_clause_mid == ast_node_type(nterm)) {
                        append_llist(list, nterm);
                        // state is still 1
                    }
                    else if(AST_else_clause_final == ast_node_type(nterm)){
                        append_llist(list, nterm);
                        state = 100;
                    }
                    else
                        state = 101; // should never happen
                }
                else
                    // not an else clause, end of list
                    state = 100;
                break;

            case 100:
                // returning a valid
                node = CREATE_AST_NODE(AST_else_clause, ast_else_clause);
                node->list = list;
                finished = true;
                break;

            case 101:
                // not found, no error
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error seen
                finished = true;
                break;

            default:
                fatal_error("invalid state in %s: (%d)", __func__, state);
        }

    }

    RETV(node);
}

/**
 * @brief
 *
 *  switch_clause
 *      = 'switch' '(' compound_reference ')' case_body
 *
 * @return ast_switch_clause*
 *
 */
ast_switch_clause* parse_switch_clause() {

    ENTER;
    ast_switch_clause* node = NULL;
    ast_compound_reference* cref;
    ast_case_body* cbod;

    if(TOK_SWITCH == TTYPE) {
        advance_token();
        if(TOK_OPAREN == TTYPE) {
            advance_token();
            if(NULL != (cref = parse_compound_reference())) {
                if(TOK_OPAREN == TTYPE) {
                    advance_token();
                    if(NULL != (cbod = parse_case_body())) {
                        node = CREATE_AST_NODE(AST_switch_clause, ast_switch_clause);
                        node->cref = cref;
                        node->cbod = cbod;
                    }
                }
                EXPECTED("a ')'");
            }
            else
                EXPECTED("a compound reference");
        }
        else {
            EXPECTED("a '('");
        }

    }
    // else no tokens are consumed

    RETV(node);
}


/**
 * @brief
 *
 *  case_item
 *      = literal_value
 *      / LITERAL_DSTR
 *      / LITERAL_SSTR
 *
 * @return ast_case_item*
 *
 */
ast_case_item* parse_case_item() {

    ENTER;
    ast_case_item* node = NULL;
    ast_literal_value* nterm;

    // QUESTION: Should I reduce this to a single token? All of the elements
    // that could be a part of a literal value are terminals.
    if(NULL != (nterm = parse_literal_value())) {
        node = CREATE_AST_NODE(AST_case_item, ast_case_item);
        node->lval = nterm;
        node->lstr = NULL;
    }
    else if((TOK_LITERAL_DSTR == TTYPE) ||
            (TOK_LITERAL_SSTR == TTYPE)) {
        advance_token();
        node = CREATE_AST_NODE(AST_case_item, ast_case_item);
        node->lstr = get_token();
        node->lval = NULL;
    }
    // else not a literal value

    RETV(node);
}


/**
 * @brief
 *
 *  case_clause
 *      = 'case' '(' case_item ')' function_body
 *
 * @return ast_case_clause*
 *
 */
ast_case_clause* parse_case_clause() {

    ENTER;
    ast_case_clause* node = NULL;
    ast_case_item* item;
    ast_function_body* fbod;

    if(TOK_CASE == TTYPE) {
        advance_token();
        if(TOK_OPAREN == TTYPE) {
            advance_token();
            if(NULL != (item = parse_case_item())) {
                if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    if(NULL != (fbod = parse_function_body())) {
                        node = CREATE_AST_NODE(AST_case_clause, ast_case_clause);
                        node->fbod = fbod;
                        node->item = item;
                    }
                    else
                        EXPECTED("a function body");
                }
                else
                    EXPECTED("a ')'");
            }
            else
                EXPECTED("a literal value");
        }
        else
            EXPECTED("a '('");
    }
    // else not a case statement

    RETV(node);
}

/**
 * @brief
 *
 *  default_clause
 *      = 'default' function_body
 *
 * @return ast_default_clause*
 *
 */
ast_default_clause* parse_default_clause() {

    ENTER;
    ast_default_clause* node = NULL;
    ast_function_body* fbod;

    if(TOK_DEFAULT == TTYPE) {
        advance_token();
        if(NULL != (fbod = parse_function_body())) {
            node = CREATE_AST_NODE(AST_default_clause, ast_default_clause);
            node->fbod = fbod;
        }
        else
            EXPECTED("a function body");
    }
    // else not a case statement

    RETV(node);
}

/**
 * @brief
 *
 *  case_body
 *      = '{' ( case_clause_list )+ ( default_clause )? '}'
 *
 * @return ast_case_body*
 *
 */
ast_case_body* parse_case_body() {

    ENTER;
    ast_case_body* node = NULL;
    ast_case_clause* ccla;
    ast_default_clause* defc;
    LList list = create_llist();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // initial state. a '{' is required
                if(TOK_OCBRACE == TTYPE) {
                    advance_token();
                    state = 2;
                }
                else {
                    EXPECTED("a '{'");
                    state = 101;
                }
                break;

            case 1:
                // a case is required
                if(NULL != (ccla = parse_case_clause())) {
                    append_llist(list, ccla);
                    state = 2;
                }
                else {
                    show_syntax("at least one case clause is required.");
                    EXPECTED("a case clause");
                    state = 101;
                }
                break;

            case 2:
                // can be a case, a default, or a '}'
                if(NULL != (ccla = parse_case_clause()))
                    append_llist(list, ccla);
                else if(NULL != (defc = parse_default_clause())) {
                    append_llist(list, defc);
                    state = 3;
                }
                else if(TOK_CCBRACE == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    show_syntax("malformed switch/case");
                    EXPECTED("a case clause, a default clause, or a '}'");
                    state = 101;
                }
                break;

            case 3:
                // seen a default, the next thing must be a '}'
                if(TOK_CCBRACE == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a '}'");
                    state = 101;
                }
                break;

            case 100:
                // no errors end. on a default
                node = CREATE_AST_NODE(AST_case_body, ast_case_body);
                node->list = list;
                finished = true;
                break;

            case 101:
                // error
                finished = true;
                break;

            default:
                fatal_error("unexpected state in %s: %d", __func__, state);
        }
    }

    RETV(node);
}


