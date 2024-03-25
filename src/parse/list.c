/**
 * @file list.c
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
 *  list_init
 *      = '[' assignment_item ( ',' assignment_item )* ']'
 *
 * @return ast_list_init*
 *
 */
ast_list_init* parse_list_init() {

    ENTER;
    ast_list_init* node = NULL;
    ast_assignment_item* nterm;
    LList list = create_llist();

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // '[' or not a match
                if(TOK_OSBRACE == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // at least one assignment item is required
                if(NULL != (nterm = parse_assignment_item())) {
                    append_llist(list, nterm);
                    state = 2;
                }
                else {
                    EXPECTED("an assignment item");
                    state = 102;
                }
                break;

            case 2:
                // a ',' or a ']' or error
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else if(TOK_CSBRACE == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a ',' or a ']'");
                    state = 102;
                }
                break;

            case 100:
                // completed parse
                node = CREATE_AST_NODE(AST_list_init, ast_list_init);
                node->list = list;
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
 *  dict_init_element
 *      = ( LITERAL_DSTRG / LITERAL_SSTRG ) ':' assignment_item
 *
 * @return ast_dict_init_element*
 *
 */
ast_dict_init_element* parse_dict_init_element() {

    ENTER;
    ast_dict_init_element* node = NULL;
    Token* key;
    ast_assignment_item* item;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // must be a literal string or it's not a match
                if((TOK_LITERAL_DSTR == TTYPE) || (TOK_LITERAL_SSTR == TTYPE)) {
                    key = get_token();
                    advance_token();
                    state = 1;
                }
                else 
                    state = 101;
                break;

            case 1:
                // a ':' or an error
                if(TOK_COLON == TTYPE) {
                    advance_token();
                    state = 2;
                }
                else {
                    EXPECTED("a ':'");
                    state = 102;
                }
                break;

            case 2:
                // an assignment item or error
                if(NULL == (item = parse_assignment_item()))
                    state = 100;
                else {
                    EXPECTED("an assignment item");
                    state = 102;
                }
                break;

            case 100:
                // completed parse
                node = CREATE_AST_NODE(AST_dict_init_element, ast_dict_init_element);
                node->key = key;
                node->item = item;
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
 *  dict_init
 *      = '[' dict_init_element ( ',' dict_init_element )* ']'
 *
 * @return ast_dict_init*
 *
 */
ast_dict_init* parse_dict_init() {

    ENTER;
    ast_dict_init* node = NULL;
    ast_dict_init_element* nterm;
    LList list = create_llist();

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // '[' or no match
                if(TOK_OSBRACE == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else 
                    state = 101;
                break;

            case 1:
                // must be a dict element or error
                if(NULL != (nterm = parse_dict_init_element())) {
                    append_llist(list, nterm);
                    state = 2;
                }
                else {
                    EXPECTED("a dictionary init element");
                    state = 102;
                }
                break;

            case 2:
                // a ',' or a ']' or an error
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else if(TOK_CSBRACE == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a ',' or a ']'");
                    state = 102;
                }
                break;

            case 100:
                // completed parse
                node = CREATE_AST_NODE(AST_dict_init, ast_dict_init);
                node->list = list;
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
 *  array_param
 *      = '[' ( expression / string_expression ) ']'
 *
 * @return ast_array_param*
 *
 */
ast_array_param* parse_array_param() {

    ENTER;
    ast_array_param* node = NULL;
    ast_node* nterm;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // '[' or not a match
                if(TOK_OSBRACE == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // must be an expression or a string_expression or an error
                if((NULL != (nterm = (ast_node*)parse_string_expr())) || 
                        (NULL != (nterm = (ast_node*)parse_expression()))) {
                    state = 2;
                }
                else {
                    EXPECTED("a string expression or expression");
                    state = 102;
                }
                break;

            case 2:
                // must be ']' or error
                if(TOK_CSBRACE == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a ']'");
                    state = 102;
                }
                break;

            case 100:
                // completed parse
                node = CREATE_AST_NODE(AST_array_param, ast_array_param);
                node->nterm = nterm;
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
 *  array_param_list
 *      = array_param (array_param)*
 *
 * @return ast_array_param_list*
 *
 */
ast_array_param_list* parse_array_param_list() {

    ENTER;
    ast_array_param_list* node = NULL;
    ast_array_param* nterm;
    LList list = create_llist();

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // '[' or not a match
                if(TOK_OSBRACE == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // must be an expression or a string_expression or an error
                if(NULL != (nterm = parse_array_param())) {
                    append_llist(list, nterm);
                }
                else {
                    state = 100;
                }
                break;

            case 100:
                // completed parse
                node = CREATE_AST_NODE(AST_array_param_list, ast_array_param_list);
                node->list = list;
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
 *  array_reference
 *      = SYMBOL array_param_list
 *
 * @return ast_array_reference*
 *
 */
ast_array_reference* parse_array_reference() {

    ENTER;
    ast_array_reference* node = NULL;
    Token* symbol;
    ast_array_param_list* param;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // 'do' or not a match
                break;

            case 100:
                // completed parse
                node = CREATE_AST_NODE(AST_array_reference, ast_array_reference);
                node->symbol = symbol;
                node->param = param;
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


