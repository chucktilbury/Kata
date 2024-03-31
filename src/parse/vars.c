/**
 * @file vars.c
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
 *  var_decl
 *      = ( 'const' )? type_name SYMBOL
 *
 * @return ast_var_decl*
 *
 */
ast_var_decl* parse_var_decl() {

    ENTER;
    ast_var_decl* node = NULL;
    bool is_const = false;

    if(TOK_CONST == TTYPE) {
        is_const = true;
        advance_token();
    }

    ast_type_name* nterm;
    if(NULL != (nterm = parse_type_name())) {
        node = CREATE_AST_NODE(AST_var_decl, ast_var_decl);
        node->is_const = is_const;
        node->type = nterm;
        finalize_token_queue();

        if(TOK_SYMBOL == TTYPE) {
            node->name = get_token();
            advance_token();
        }
        else {
            EXPECTED("a symbol");
            node = NULL;
        }
    }
    else if(is_const) {
        EXPECTED("a type name");
        node = NULL;
    }
    // else it's not a var_decl

    RETV(node);
}

/**
 * @brief
 *
 *  var_decl_list
 *      = '(' ( var_decl ( ',' var_decl )* )? ')'
 *
 * @return ast_var_decl_list*
 *
 */
ast_var_decl_list* parse_var_decl_list() {

    ENTER;
    ast_var_decl_list* node = NULL;
    ast_var_decl* nterm;
    LinkList* list = create_link_list();
    int state = 0;
    bool finished = false;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0 :
                // if there is no '(' then it's not a var decl list.
                if(TOK_OPAREN == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // if there is no var decl, then there must be a ')'
                if(NULL != (nterm = parse_var_decl())) {
                    append_link_list(list, nterm);
                    state = 2;
                }
                else if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else
                    state = 101;
                break;

            case 2:
                // if the token is a ',', then expect another item, else finished
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    state = 3;
                }
                else if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a ')' or a ','");
                    state = 102;
                }
                break;

            case 3:
                // must be an var_decl, or an error
                if(NULL != (nterm = parse_var_decl())) {
                    append_link_list(list, nterm);
                    state = 2;
                }
                else {
                    EXPECTED("a variable declaration");
                    state = 102;
                }
                break;

            case 100:
                // var_decl is complete
                node = CREATE_AST_NODE(AST_var_decl_list, ast_var_decl_list);
                node->list = list;
                finished = true;
                break;

            case 101:
                // not a var_decl, no error
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // not a var_decl, is an error
                node = NULL;
                finished = true;
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
 *  var_definition
 *      = ( 'var' / 'variable' ) var_decl ( '=' assignment_item )?
 *
 * @return ast_var_definition*
 *
 */
ast_var_definition* parse_var_definition() {

    ENTER;
    ast_var_definition* node = NULL;
    ast_var_decl* nterm;
    ast_assignment_item* item;
    void* post = post_token_queue();

    if(TOK_VAR == TTYPE || TOK_VARIABLE == TTYPE) {
        advance_token();
        if(NULL != (nterm = parse_var_decl())) {
            node = CREATE_AST_NODE(AST_var_definition, ast_var_definition);
            node->is_assigned = false;
            node->type = nterm;

            // optional assignment
            if(TOK_ASSIGN == TTYPE) {
                advance_token();

                if(NULL != (item = parse_assignment_item())) {
                    node->item = item;
                    node->is_assigned = true;
                }
                else {
                    EXPECTED("an assignment item");
                    node = NULL;
                }
            }

            finalize_token_queue();
        }
        else {
            // nope
            EXPECTED("a variable declaration");
        }
    }
    else {
        // not an error
        reset_token_queue(post);
    }

    RETV(node);
}



