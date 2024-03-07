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
#define USE_TRACE 1
#include "util.h"
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

    if(TOK_LITERAL_SSTR == token_type(get_token())) {
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

/**
 * @brief
 *
 *  string_expr
 *      = string_expr_item ( '+' string_expr_item )*
 *
 * @return ast_string_expr*
 *
 */
ast_string_expr* parse_string_expr() {

    ENTER;
    ast_string_expr* node = NULL;
    ast_node* nterm;
    LList list = create_llist();
    int state = 0;
    bool finished = false;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // first entry must be a string expr item or no error
                if(NULL != (nterm = (ast_node*)parse_string_expr_item())) {
                    append_llist(list, nterm);
                    state = 2;
                }
                else 
                    state = 101;
                break;

            case 1:
                // expecting a str expr item or an error
                if(NULL != (nterm = (ast_node*)parse_string_expr_item())) {
                    append_llist(list, nterm);
                    state = 2;
                }
                else {
                    EXPECTED("a string expression item");
                    state = 102;
                }
                break;

            case 2:
                // expecting a '+' or something else. if it's something else, 
                // then finished parsing the string expr
                if(TOK_ADD == token_type(get_token())) {
                    state = 1;
                    advance_token();
                }
                else {
                    state = 100;
                }
                break;

            case 100:
                // finished parsing a valid expr
                node = CREATE_AST_NODE(AST_string_expr, ast_string_expr);
                node->list = list;
                finalize_token_queue();
                finished = true;
                break;

            case 101:
                // not a string expr, not an error, may have read tokens.
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // is an error
                finished = true;
                break;

            default:
                // should never happen. 
                fatal_error("invalid state in %s(): %d", __func__, state);
            
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  formatted_strg
 *      = LITERAL_DSTRG (expression_list)?
 *
 * @return ast_formatted_strg*
 *
 */
ast_formatted_strg* parse_formatted_strg() {

    ENTER;
    ast_formatted_strg* node = NULL;
    ast_expression_list* nterm;

    if(TOK_LITERAL_DSTR == token_type(get_token())) {
        node = CREATE_AST_NODE(AST_formatted_strg, ast_formatted_strg);
        node->str = get_token();
        advance_token();

        if(NULL != (nterm = parse_expression_list())) {
            node->exprs = nterm;
        }
        else 
            node->exprs = NULL;
    }
    // else no tokens are consumed

    RETV(node);
}

