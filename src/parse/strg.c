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
ast_string_literal* parse_string_literal(void) {

    ENTER;
    ast_string_literal* node = NULL;
    ast_formatted_strg* fstr;
    void* post = post_token_queue();

    if(TOK_LITERAL_SSTR == TTYPE) {
        TRACE("sstr");
        TRACE_TERM(get_token());
        node = CREATE_AST_NODE(AST_string_literal, ast_string_literal);
        node->literal = get_token();
        node->fmt = NULL;
        advance_token();
    }
    else if(NULL != (fstr = parse_formatted_strg())) {
        TRACE("formatted str");
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
 *  formatted_strg
 *      = LITERAL_DSTRG ( expression_list )?
 *
 * @return ast_formatted_strg*
 *
 */
ast_formatted_strg* parse_formatted_strg(void) {

    ENTER;
    ast_formatted_strg* node = NULL;
    ast_expression_list* nterm;

    if(TOK_LITERAL_DSTR == TTYPE) {
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

