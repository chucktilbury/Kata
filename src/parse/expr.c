/**
 * @file expr.c
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

// This is used to detect unary operators. If it is true, and an operator is
// found then it could be unary. Only the '!' and the '-' can be unary, all
// others are errors if the flag is set.
static bool flag = true;

static int get_oper_type(TokenType type) {

    switch(type) {
        case TOK_OR:
        case TOK_AND:
        case TOK_EQU:
        case TOK_NEQU:
        case TOK_LORE:
        case TOK_GORE:
            return 1;
        case TOK_OPBRACE:
        case TOK_CPBRACE:
            return 2;
        case TOK_ADD:
        case TOK_SUB:
        case TOK_DIV:
        case TOK_MUL:
        case TOK_MOD:
        case TOK_CARAT:
            return 3;
        case TOK_NOT:
        case TOK_UNARY_MINUS:
            return 4;
        default:
            // not an operator
            return 0;
    }
}

static int get_prec(TokenType type) {

    switch(type) {
        case TOK_OR:
            return 10;
        case TOK_AND:
            return 20;
        case TOK_EQU:
        case TOK_NEQU:
            return 30;
        case TOK_LORE:
        case TOK_GORE:
        case TOK_OPBRACE:
        case TOK_CPBRACE:
            return 40;
        case TOK_ADD:
        case TOK_SUB:
            return 50;
        case TOK_DIV:
        case TOK_MUL:
        case TOK_MOD:
            return 60;
        case TOK_CARAT:
            return 70;
        case TOK_NOT:
        case TOK_UNARY_MINUS:
            return 80;
        default:
            // no precidence
            return 0;
    }
}

static bool get_assoc(TokenType type) {

    switch(type) {
        case TOK_NOT:
        case TOK_UNARY_MINUS:
        case TOK_CARAT:
            return false;   // right to left
        default:
            return true;    // left to right
    }
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

    switch(token_type(tok)) {
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
        case TOK_OPAREN:
        case TOK_CARAT:
            // these operators must not be unary
            if(!flag) {
                node = CREATE_AST_NODE(AST_operator, ast_operator);
                node->tok = tok;
                finalize_token();
                advance_token();
                flag = true;
            }
            else
                show_syntax_error("the %s operator must not be unary", tok_to_str(tok));
            break;

        case TOK_SUB:
            // the '-' can be unary
            node = CREATE_AST_NODE(AST_operator, ast_operator);
            node->tok = tok;
            if(flag)
                node->tok->type = TOK_UNARY_MINUS;
            finalize_token();
            advance_token();
            flag = true;
            break;

        case TOK_NOT:
            // the '!' or 'not' must be unary
            if(flag) {
                node = CREATE_AST_NODE(AST_operator, ast_operator);
                node->tok = tok;
                finalize_token();
                advance_token();
                flag = true;
            }
            else
                show_syntax_error("the not operator must be unary");
            break;

        case TOK_CPAREN:
            node = CREATE_AST_NODE(AST_operator, ast_operator);
            node->tok = tok;
            finalize_token();
            advance_token();
            flag = false;
            break;
        default:
            break;
    }

    RETV(node);
}

/**
 * @brief
 *
 *  cast_statement
 *      = type_name ':' expression
 *
 * @return ast_cast_statement*
 *
 */
ast_cast_statement* parse_cast_statement() {

    ENTER;
    ast_cast_statement* node = NULL;
    void* post = post_token_queue();
    ast_type_name* type;
    ast_expression* expr;

    if(NULL != (type = parse_type_name())) {
        if(TOK_COLON == token_type(get_token())) {
            finalize_token();
            advance_token();

            if(NULL != (expr = parse_expression())) {
                node = CREATE_AST_NODE(AST_cast_statement, ast_cast_statement);
                node->type = type;
                node->expr = expr;
            }
            else
                EXPECTED("an expression");
        }
        else // not a cast statement, not an error
            reset_token_queue(post);
    }
    else // not a cast, not an error
        reset_token_queue(post);

    RETV(node);
}

/**
 * @brief
 *
 *  expr_primary
 *      = literal_value
 *      / compound_reference
 *      / cast_statement
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
            (NULL != (nterm = (ast_node*)parse_compound_reference())) ||
            (NULL != (nterm = (ast_node*)parse_cast_statement()))) {

        node = CREATE_AST_NODE(AST_expr_primary, ast_expr_primary);
        node->nterm = nterm;
        flag = false;
        finalize_token_queue();
    }
    else
        reset_token_queue(post);

    RETV(node);
}

/**
 * @brief
 *
 *  expression_list
 *      = '(' (expression ( ',' expression )*)? ')'
 *
 * @return ast_expression_list*
 *
 */
ast_expression_list* parse_expression_list() {

    ENTER;
    ast_expression_list* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  assignment_item
 *      = expression
 *      / list_init
 *      / dict_init
 *      / string_expr
 *      / cast_statement
 *
 * @return ast_assignment_item*
 *
 */
ast_assignment_item* parse_assignment_item() {

    ENTER;
    ast_assignment_item* node = NULL;
    ast_node* nterm;
    void* post = post_token_queue();

    if((NULL != (nterm = (ast_node*)parse_expression())) ||
            (NULL != (nterm = (ast_node*)parse_list_init())) ||
            (NULL != (nterm = (ast_node*)parse_dict_init())) ||
            (NULL != (nterm = (ast_node*)parse_string_expr())) ||
            (NULL != (nterm = (ast_node*)parse_cast_statement()))) {

        node = CREATE_AST_NODE(AST_assignment_item, ast_assignment_item);
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
 *  assignment
 *      = compound_reference '=' assignment_item
 *      / compound_reference '+=' expression
 *      / compound_reference '-=' expression
 *      / compound_reference '*=' expression
 *      / compound_reference '/=' expression
 *      / compound_reference '%=' expression
 *
 * @return ast_assignment*
 *
 */
ast_assignment* parse_assignment() {

    ENTER;
    ast_assignment* node = NULL;

    RETV(node);
}

/**
 * @brief
 *
 *  expression
 *      = expr_and 'or' expr_or
 *      / expr_equ 'and' expr_and
 *      / expr_mag '==' expr_equ
 *      / expr_mag '!=' expr_equ
 *      / expr_term '<' expr_mag
 *      / expr_term '>' expr_mag
 *      / expr_term '<=' expr_mag
 *      / expr_term '>=' expr_mag
 *      / expr_fact '+' expr_term
 *      / expr_fact '-' expr_term
 *      / expr_pow '*' expr_fact
 *      / expr_pow '/' expr_fact
 *      / expr_pow '%' expr_fact
 *      / expr_unary '^' expr_pow
 *      / '!' expr_primary
 *      / '-' expr_primary
 *      / '(' expression ')'
 *
 * @return ast_expression*
 *
 */
ast_expression* parse_expression() {

    ENTER;
    ast_expression* node = NULL;
    PtrList* stack = create_ptr_list();
    PtrList* queue = create_ptr_list();
    ast_node* nterm;
    void* post = post_token_queue();
    int type = 0;

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // entry point; make sure it's an expression
                if(NULL != (nterm = (ast_node*)parse_operator()))
                    state = 3;
                else if (NULL != (nterm = (ast_node*)parse_expr_primary()))
                    state = 8;
                else
                    state = 101;
                break;

            case 1:
                // must have a primary or a unary
                if(NULL != (nterm = (ast_node*)parse_operator()))
                    state = 3;
                else if (NULL != (nterm = (ast_node*)parse_expr_primary()))
                    state = 8;
                else
                    state = 100;
                break;

            case 2:
                // must have an operator
                break;

            case 3: {
                    // handle an operator
                    TokenType type = token_type(((ast_operator*)nterm)->tok);
                    switch(type) {
                        case TOK_OPAREN:
                            state = 4;
                            break;
                        case TOK_CPAREN:
                            state = 5;
                            break;
                        default:
                            if(get_assoc(type))
                                state = 6;
                            else
                                state = 7;
                            break;
                    }
                }
                break;

            case 5:
                // operator is close paren
                state = 1;
                while(true) {
                    ast_operator* op = pop_ptr_list(stack);
                    if(NULL == op) {
                        show_syntax_error("imbalanced parens in expression");
                        state = 102;
                        break;
                    }
                    else if(TOK_OPAREN == token_type(op->tok)) {
                        state = 1;
                        break;
                    }
                    else
                        add_ptr_list(queue, op);
                }
                break;

            case 6: {
                    // operator is left assoc
                    ast_operator* op = peek_ptr_list(stack);
                    Token* tok = op->tok;

                    state = 1;
                }
                break;

            case 7:
                // operator it right assoc
                state = 1;
                break;

            case 4:
            case 8:
                // handle a primary or an open paren
                add_ptr_list(queue, nterm);
                state = 1;
                break;

            case 100:
                // finished expression
                node = CREATE_AST_NODE(AST_expression, ast_expression);
                node->list = queue;
                node->expr_type = type;
                finalize_token_queue();
                finished = true;
                break;

            case 101:
                // not an expression, not an error;
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // exit on a syntax error;
                node = NULL;
                finished = true;
                break;

            default:
                // cannot happen
                fatal_error("invalid state in %s: %d", __func__, state);
                break;
        }
    }

    RETV(node);
}

/*
    while((NULL != (nterm = (ast_node*)parse_operator())) ||
                (NULL != (nterm = (ast_node*)parse_expr_primary()))) {

            if(AST_expr_primary == ast_node_type(nterm)) {
                add_ptr_list(queue, nterm);
                finalize_token_queue();
            }
            else {
                ast_operator* oper = (ast_operator*)nterm;
                Token* oper_tok = oper->tok;

                if(TOK_OPAREN == token_type(oper_tok)) {
                    // open paren
                    push_ptr_list(stack, nterm);
                    finalize_token_queue();
                }
                else if(TOK_CPAREN == token_type(oper_tok)) {
                    // close paren. pop until open paren
                    while(true) {
                        ast_operator* item = pop_ptr_list(stack);
                        if(NULL == item) {
                            EXPECTED("a ')'");
                            RETV(NULL);
                        }
                        else if(TOK_OPAREN == token_type(item->tok)) {
                            break; // finished and discard paren
                        }
                        else {
                            // copy it to the queue
                            add_ptr_list(queue, item);
                        }
                    }
                }
                else if(get_assoc(token_type(oper_tok))) {
                    // is left assoc
                    ast_operator* item = peek_ptr_list(stack);
                    while(item != NULL &&
                            (get_prec(token_type(item->tok)) >=
                            get_prec(token_type(oper_tok)))) {
                        add_ptr_list(queue, pop_ptr_list(stack));
                    }

                    push_ptr_list(stack, oper);
                }
                else {
                    // is right assoc
                    ast_operator* item = peek_ptr_list(stack);
                    while(item != NULL &&
                            (get_prec(token_type(item->tok)) >
                            get_prec(token_type(oper_tok)))) {
                        add_ptr_list(queue, pop_ptr_list(stack));
                    }

                    push_ptr_list(stack, oper);
                }
            }
        }
        else {
            node =
            break;
        }

    }

    RETV(node);
}

*/
