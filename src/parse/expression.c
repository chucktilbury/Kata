/**
 * @file expression.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "trace.h"
#include "parse.h"
#include "scanner.h"

static bool flag = true;
static int pcount = 0;

/**
 * @brief
 *
 *  expr_primary
 *      = literal_value
 *      / compound_reference
 *
 * @return ast_expr_primary*
 *
 */
ast_expr_primary* parse_expr_primary() {

    ENTER;
    ast_expr_primary* node = NULL;
    ast_node* nterm;
    void* post = post_token_queue();

    if((NULL != (nterm = (ast_node*)parse_literal_value())) ||
            (NULL != (nterm = (ast_node*)parse_compound_reference()))) {

        node = CREATE_AST_NODE(AST_expr_primary, ast_expr_primary);
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
 *  ast_operator
 *      = AND
 *      / OR
 *      / '=='
 *      / '!='
 *      / '<'
 *      / '>'
 *      / '<='
 *      / '>='
 *      / '+'
 *      / '-'
 *      / '*'
 *      / '/'
 *      / '%'
 *      / '^'
 *      / UNARY_MINUS
 *      / '!'
 *      / '('
 *      / ')'
 *
 * @return ast_operator*
 *
 */
ast_operator* parse_operator() {

    ENTER;
    ast_operator* node = NULL;
    Token* tok = get_token();

    switch(TTYPE) {
        case TOK_AND:
        case TOK_OR:
        case TOK_LORE:
        case TOK_GORE:
        case TOK_OPBRACE:
        case TOK_CPBRACE:
        case TOK_EQU:
        case TOK_NEQU:
        case TOK_ADD:
        case TOK_ASSIGN:
        case TOK_DIV:
        case TOK_MUL:
        case TOK_MOD:
        case TOK_CARAT:
            // these operators must not be unary
            if(!flag) {
                TRACE_TERM(tok);
                node = CREATE_AST_NODE(AST_operator, ast_operator);
                node->tok = tok;
                advance_token();
                flag = true;
            }
            else
                show_syntax("the %s operator must not be unary", tok_to_str(tok));
            break;

        case TOK_SUB:
            // the '-' can be unary
            node = CREATE_AST_NODE(AST_operator, ast_operator);
            node->tok = tok;
            if(flag)
                node->tok->type = TOK_UNARY_MINUS;
            advance_token();
            flag = true;
            break;

        case TOK_NOT:
            // the '!' or 'not' must be unary
            if(flag) {
                node = CREATE_AST_NODE(AST_operator, ast_operator);
                node->tok = tok;
                advance_token();
                flag = true;
            }
            else
                show_syntax("the not operator must be unary");
            break;

        case TOK_OPAREN:
            node = CREATE_AST_NODE(AST_operator, ast_operator);
            node->tok = tok;
            advance_token();
            flag = true;
            pcount++;
            break;

        case TOK_CPAREN:
            node = CREATE_AST_NODE(AST_operator, ast_operator);
            node->tok = tok;
            advance_token();
            flag = false;
            pcount--;
            break;

        default:
            break;
    }

    RETV(node);
}

/**
 * @brief This parses an expression to make sure that the syntax is correct, 
 * not to prepare to solve it. This expression will be emitted more or less 
 * directly to the output.
 * 
 * @return ast_expression* 
 */
ast_expression* parse_expression() {

    ENTER;

    ast_expression* node = NULL;
    LinkList* list = create_link_list();
    int expr_type;

    void* post = post_token_queue();
    int pcount = 0;
    bool uflag = true;
    bool finished = false;
    int state = 0;

    while(!finished) {
        switch(state) {
            case 0:
                // 
                break;

            case 100:
                // verify the state is correct to exit.
                if(pcount == 0)
                    state = 101;
                else {
                    show_syntax("parentheses are imbalanced");
                    state = 103;
                }
                break;

            case 101:
                // production is complete.
                node = CREATE_AST_NODE(AST_expression, ast_expression);
                node->expr_type = expr_type;
                node->list = list;
                finished = true;
                break;

            case 102:
                // not a match
                reset_token_queue(post);
                finished = true;
                break;

            case 103:
                // error
                finished = true;
                break;

            default:
                fatal_error("unknown state is %s: %d", __func__, state);
        }
    }

    RETV(node);
}

