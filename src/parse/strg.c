/**
 * @file string.c
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
 *  string_literal
 *      = LITERAL_SSTRG
 *      / formatted_strg
 *
 * @return ast_string_literal*
 *
 */
ast_string_literal* parse_string_literal() {

    ENTER;
    ast_string_literal* node = NULL;
    ast_formatted_strg* fstr;
    void* post = post_token_queue();

    if(TOK_LITERAL_SSTR == TTYPE) {
        node = CREATE_AST_NODE(AST_string_literal, ast_string_literal);
        node->literal = get_token();
        node->fmt = NULL;
        advance_token();
    }
    else if(NULL != (fstr = parse_formatted_strg())) {
        node = CREATE_AST_NODE(AST_string_literal, ast_string_literal);
        node->literal = NULL;
        node->fmt = fstr;
    }
    else
        reset_token_queue(post);

    RETV(node);
}

/**
 * @brief
 *
 *  string_expr_item
 *      = string_literal
 *      / compound_reference
 *      / literal_value
 *
 * @return ast_string_expr_item*
 *
 */
ast_string_expr_item* parse_string_expr_item() {

    ENTER;
    ast_string_expr_item* node = NULL;
    ast_node* nterm;
    void* post = post_token_queue();

    if((NULL != (nterm = (ast_node*)parse_string_literal())) ||
            (NULL != (nterm = (ast_node*)parse_compound_reference())) ||
            (NULL != (nterm = (ast_node*)parse_literal_value()))) {

        node = CREATE_AST_NODE(AST_string_expr_item, ast_string_expr_item);
        node->nterm = nterm;
        finalize_token_queue();
    }
    else
        reset_token_queue(post);

    RETV(node);
}

// TODO: Make production match grammar
/**
 * @brief
 *
 *  string_expr_list
 *      = '(' string_expr_item ( ',' string_expr_item )* ')'
 *
 * @return ast_string_expr*
 *
 */
ast_string_expr_list* parse_string_expr_list() {

    ENTER;
    ast_string_expr_list* node = NULL;
    ast_node* nterm;
    LinkList* list = create_link_list();

    int state = 0;
    bool finished = false;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // if not a '(' then it's not a str expr list
                TRACE("state: %d", state);
                if(TOK_OPAREN == TTYPE) {
                    state = 1;
                    advance_token();
                }
                else 
                    state = 101;
                break;

            case 1:
                // can be an item or a ')' or error
                TRACE("state: %d", state);
                if(TOK_CPAREN == TTYPE) {
                    state = 100;
                    advance_token();
                }
                else if(NULL != (nterm = (ast_node*)parse_string_expr_item())) {
                    append_link_list(list, nterm);
                    state = 2;
                }
                else {
                    EXPECTED("a string format item");
                    state = 102;
                }
                break;

            case 2:
                // must be a ',' or a ')' or error
                TRACE("state: %d", state);
                if(TOK_COMMA == TTYPE) {
                    state = 3;
                    advance_token();
                }
                else if(TOK_CPAREN == TTYPE) {
                    state = 100;
                    advance_token();
                }
                else {
                    EXPECTED("a ',' or a ')'");
                    state = 102;
                }
                break;

            case 3:
                // must be an item or error
                TRACE("state: %d", state);
                if(NULL != (nterm = (ast_node*)parse_string_expr_item()))  {
                    append_link_list(list, nterm);
                    state = 2;
                }
                else {
                    EXPECTED("a string format item");
                    state = 102;
                }
                break;

            case 100:
                // create the node and return it
                TRACE("state: %d", state);
                node = CREATE_AST_NODE(AST_string_expr_list, ast_string_expr_list);
                node->list = list;
                finished = true;
                break;

            case 101:
                // not a match
                TRACE("state: %d", state);
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                TRACE("state: %d", state);
                finished = true;
                break;
            
            default:
                fatal_error("unknown state in %s: %d", __func__, state);
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  formatted_strg
 *      = LITERAL_DSTRG ( string_expr_list )?
 *
 * @return ast_formatted_strg*
 *
 */
ast_formatted_strg* parse_formatted_strg() {

    ENTER;
    ast_formatted_strg* node = NULL;
    ast_string_expr_list* nterm;

    if(TOK_LITERAL_DSTR == TTYPE) {
        node = CREATE_AST_NODE(AST_formatted_strg, ast_formatted_strg);
        node->str = get_token();
        advance_token();

        if(NULL != (nterm = parse_string_expr_list())) {
            node->exprs = nterm;
        }
        else
            node->exprs = NULL;
    }
    // else no tokens are consumed

    RETV(node);
}

