/**
 * @file list.c
 *
 * @brief For a list init, it must contain either list init items or dict 
 * init items. Rather than make the 2 different ones dependent on each other,
 * they are combined into a list init production.
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
 *  list_init
 *      = '[' assignment_item ( ',' assignment_item )* ']'
 *      / '[' dict_init_element ( ',' dict_init_element )* ']'
 *
 * @return ast_list_init*
 *
 */
ast_list_init* parse_list_init(void) {

    ENTER;
    ast_list_init* node = NULL;
    ast_node* nterm;
    LinkList* list = create_link_list();

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // '[' or not a match
                TRACE("state = %d", state);
                if(TOK_OSBRACE == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 102;
                break;

            case 1:
                // at least one assignment item is required. determine if it's 
                // a list or a dictionary
                TRACE("state = %d", state);
                if(NULL != (nterm = (ast_node*)parse_dict_init_element())) {
                    append_link_list(list, nterm);
                    state = 5;
                }
                else if(NULL != (nterm = (ast_node*)parse_assignment_item())) {
                    append_link_list(list, nterm);
                    state = 3;
                }
                else {
                    EXPECTED("an assignment item or a dict element");
                    state = 103;
                }
                break;

            case 2:
                // at least one assignment item is required. determine if it's 
                // a list or a dictionary
                TRACE("state = %d", state);
                if(NULL != (nterm = (ast_node*)parse_assignment_item())) {
                    append_link_list(list, nterm);
                    state = 3;
                }
                else {
                    EXPECTED("an assignment item");
                    state = 103;
                }
                break;

            case 3:
                // a ',' or a ']' or error in the context of a list init
                TRACE("state = %d", state);
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    state = 2;
                }
                else if(TOK_CSBRACE == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    if(TOK_COLON == TTYPE)
                        SYNTAX("expected a list init item but got a dict init item");
                    else
                        EXPECTED("a ',' or a ']'");
                    state = 103;
                }
                break;

            case 4:
                // at least one assignment item is required. determine if it's 
                // a list or a dictionary
                TRACE("state = %d", state);
                if(NULL != (nterm = (ast_node*)parse_dict_init_element())) {
                    append_link_list(list, nterm);
                    state = 5;
                }
                else {
                    EXPECTED("a dict element");
                    state = 103;
                }
                break;

            case 5:
                // a ',' or a ']' or error in the context of a dict init
                TRACE("state = %d", state);
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    state = 4;
                }
                else if(TOK_CSBRACE == TTYPE) {
                    advance_token();
                    state = 101;
                }
                else {
                    EXPECTED("a ',' or a ']'");
                    state = 103;
                }
                break;

            case 100:
                // completed parse for list init
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_list_init, ast_list_init);
                node->list = list;
                finished = true;
                break;

            case 101:
                // completed parse for dict init
                TRACE("state = %d", state);
                node = (ast_list_init*)CREATE_AST_NODE(AST_dict_init, ast_dict_init);
                node->list = list;
                finished = true;
                break;

            case 102:
                // not a match
                TRACE("state = %d", state);
                reset_token_queue(post);
                finished = true;
                break;

            case 103:
                // error
                TRACE("state = %d", state);
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
ast_dict_init_element* parse_dict_init_element(void) {

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
                TRACE("state = %d", state);
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
                TRACE("state = %d", state);
                if(TOK_COLON == TTYPE) {
                    advance_token();
                    state = 2;
                }
                else {
                    // EXPECTED("a ':'");
                    // state = 102;
                    state = 101;
                }
                break;

            case 2:
                // an assignment item or error
                TRACE("state = %d", state);
                if(NULL != (item = parse_assignment_item()))
                    state = 100;
                else {
                    EXPECTED("an assignment item");
                    state = 102;
                }
                break;

            case 100:
                // completed parse
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_dict_init_element, ast_dict_init_element);
                node->key = key;
                node->item = item;
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

    RETV(node);
}

/**
 * @brief
 *
 *  array_param
 *      = '[' ( expression / string_literal ) ']'
 *
 * @return ast_array_param*
 *
 */
ast_array_param* parse_array_param(void) {

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
                TRACE("state = %d", state);
                if(TOK_OSBRACE == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // must be an expression or a string_expression or an error
                TRACE("state = %d", state);
                if(
                    (NULL != (nterm = (ast_node*)parse_string_literal())) ||
                    (NULL != (nterm = (ast_node*)parse_expression()))) {

                    state = 2;
                }
                else {
                    EXPECTED("a string or expression");
                    state = 102;
                }
                break;

            case 2:
                // must be ']' or error
                TRACE("state = %d", state);
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
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_array_param, ast_array_param);
                node->nterm = nterm;
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
ast_array_param_list* parse_array_param_list(void) {

    ENTER;
    ast_array_param_list* node = NULL;
    ast_array_param* nterm;
    LinkList* list = create_link_list();

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // must be an expression or a string_expression or an error
                TRACE("state = %d", state);
                if(NULL != (nterm = parse_array_param())) {
                    append_link_list(list, nterm);
                    state = 1;
                }
                else {
                    state = 101;
                }
                break;

            case 1:
                // must be an expression or a string_expression or an error
                TRACE("state = %d", state);
                if(NULL != (nterm = parse_array_param())) {
                    append_link_list(list, nterm);
                }
                else {
                    state = 100;
                }
                break;

            case 100:
                // completed parse
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_array_param_list, ast_array_param_list);
                node->list = list;
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

    RETV(node);
}

#if 0
/**
 * @brief
 *
 *  array_reference
 *      = SYMBOL array_param_list
 *
 * @return ast_array_reference*
 *
 */
ast_array_reference* parse_array_reference(void) {

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
                // SYMBOL or not a match
                TRACE("state = %d", state);
                if(TOK_SYMBOL == TTYPE) {
                    advance_token();
                    symbol = get_token();
                    state = 1;
                }
                else 
                    state = 101;
                break;

            case 1:
                // must be an array param list or not a match
                TRACE("state = %d", state);
                if(NULL != (param = parse_array_param_list())) {
                    state = 100;
                }
                else 
                    state = 101;
                break;

            case 100:
                // completed parse
                TRACE("state = %d", state);
                node = CREATE_AST_NODE(AST_array_reference, ast_array_reference);
                node->symbol = symbol;
                node->param = param;
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

    RETV(node);
}
#endif

