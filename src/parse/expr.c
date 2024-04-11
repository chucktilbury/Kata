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
#include "trace.h"
#include "parse.h"
#include "scanner.h"

// This is used to detect unary operators. If it is true, and an operator is
// found then it could be unary. Only the '!' and the '-' can be unary, all
// others are errors if the flag is set.
static bool flag = true;

// Boolean expressions take precedence over arithmetic. Any boolean
// operator in the expression causes the whole expression to be a boolean,
// no matter what else is in it.
static int get_oper_type(TokenType type, TokenType crnt) {

    switch(type) {
        case TOK_OR:
        case TOK_AND:
        case TOK_EQU:
        case TOK_NEQU:
        case TOK_LORE:
        case TOK_GORE:
        case TOK_NOT:
            return 1;
        case TOK_ADD:
        case TOK_SUB:
        case TOK_DIV:
        case TOK_MUL:
        case TOK_MOD:
        case TOK_CARAT:
        case TOK_UNARY_MINUS:
            if(crnt == 1)
                return 1;
            else
                return 2;
        case TOK_OPBRACE:
        case TOK_CPBRACE:
            return crnt;
        default:
            // not an operator
            return 0;
    }
}

static int get_prec(TokenType type) {

    int prec = 0;

    switch(type) {
        case TOK_OR:
            prec = 10;
            break;
        case TOK_AND:
            prec = 20;
            break;
        case TOK_EQU:
        case TOK_NEQU:
            prec = 30;
            break;
        case TOK_LORE:
        case TOK_GORE:
        case TOK_OPBRACE:
        case TOK_CPBRACE:
            prec = 40;
            break;
        case TOK_ADD:
        case TOK_SUB:
            prec = 50;
            break;
        case TOK_DIV:
        case TOK_MUL:
        case TOK_MOD:
            prec = 60;
            break;
        case TOK_CARAT:
            prec = 70;
            break;
        case TOK_NOT:
        case TOK_UNARY_MINUS:
            prec = 80;
            break;
        default:
            // no precedence
            TRACE("no precedence");
            prec = 0;
            break;
    }
    TRACE("prec: %d", prec);
    return prec;
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
        case TOK_CARAT:
            // these operators must not be unary
            if(!flag) {
                TRACE_TERM(tok);
                node = CREATE_AST_NODE(AST_operator, ast_operator);
                node->tok = tok;
                //advance_token();
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
            //advance_token();
            flag = true;
            break;

        case TOK_NOT:
            // the '!' or 'not' must be unary
            if(flag) {
                node = CREATE_AST_NODE(AST_operator, ast_operator);
                node->tok = tok;
                //advance_token();
                flag = true;
            }
            else
                show_syntax("the not operator must be unary");
            break;

        case TOK_OPAREN:
            node = CREATE_AST_NODE(AST_operator, ast_operator);
            node->tok = tok;
            //advance_token();
            flag = true;
            break;

        case TOK_CPAREN:
            node = CREATE_AST_NODE(AST_operator, ast_operator);
            node->tok = tok;
            //advance_token();
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
        if(TOK_COLON == TTYPE) {
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
    ast_expression* expr;
    LinkList* list = create_link_list();
    bool finished = false;
    int state = 0;
    int pcount = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                TRACE("state: %d, len: %d", state, len_link_list(list));
                // open paren required, or no error, no tokens consumed
                if(TOK_OPAREN == TTYPE) {
                    pcount++;
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;
            case 1:
                TRACE("state: %d, len: %d", state, len_link_list(list));
                // expression is optional for first time through
                if(NULL != (expr = parse_expression())) {
                    append_link_list(list, expr);
                    state = 2;
                }
                else if(TOK_CPAREN == TTYPE) {
                    pcount--;
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("an expression or a ')'");
                    state = 102;
                }
                break;
            case 2:
                TRACE("state: %d, len: %d", state, len_link_list(list));
                TRACE_TERM(get_token());
                // require an ',' or a ')'
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    state = 3;
                }
                else if(TOK_CPAREN == TTYPE) {
                    pcount--;
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a ',' or a ')'");
                    state = 102;
                }
                break;
            case 3:
                TRACE("state: %d, len: %d", state, len_link_list(list));
                // expression is required after a ','
                if(NULL != (expr = parse_expression())) {
                    append_link_list(list, expr);
                    state = 2;
                }
                else {
                    EXPECTED("an expression");
                    state = 102;
                }
                break;

            case 100:
                TRACE("state: %d, len: %d, pcount: %d", state, len_link_list(list), pcount);
                // finished no error
                node = CREATE_AST_NODE(AST_expression_list, ast_expression_list);
                node->list = list;
                finalize_token_queue();
                finished = true;
                break;

            case 101:
                TRACE("state: %d, len: %d", state, len_link_list(list));
                // finished no expression with no error
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                TRACE("state: %d, len: %d", state, len_link_list(list));
                // finished with an error
                node = NULL;
                finished = true;
                break;

            default:
                fatal_error("unexpected state in %s: %d", __func__, state);
        }
    }

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
 *      / function_assignment
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
            (NULL != (nterm = (ast_node*)parse_cast_statement())) ||
            (NULL != (nterm = (ast_node*)parse_function_assignment()))) {

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
 *      / compound_reference '+=' assignment_item
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
    ast_compound_reference* cref;
    ast_node* nterm;
    void* post = post_token_queue();

    if(NULL != (cref = parse_compound_reference())) {
        Token* tok = get_token();
        switch(token_type(tok)) {
            case TOK_SUB_ASSIGN:
            case TOK_MUL_ASSIGN:
            case TOK_DIV_ASSIGN:
            case TOK_MOD_ASSIGN:
                if(NULL != (nterm = (ast_node*)parse_expression())) {
                    node = CREATE_AST_NODE(AST_assignment, ast_assignment);
                    node->lhs = cref;
                    node->rhs = nterm;
                    node->oper = tok;
                    advance_token();
                    finalize_token_queue();
                }
                else
                    EXPECTED("an expression");
                break;
            case TOK_ADD_ASSIGN:
            case TOK_ASSIGN:
                if(NULL != (nterm = (ast_node*)parse_assignment_item())) {
                    node = CREATE_AST_NODE(AST_assignment, ast_assignment);
                    node->lhs = cref;
                    node->rhs = nterm;
                    node->oper = tok;
                    advance_token();
                    finalize_token_queue();
                }
                else
                    EXPECTED("an assignment item");
                break;
            default:
                // another rule might match
                reset_token_queue(post);
                RETV(NULL);
        }
    }
    else
        reset_token_queue(post);

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
    LinkList* stack = create_link_list();
    LinkList* queue = create_link_list();
    ast_node* nterm;
    void* post = post_token_queue();
    int expr_type = 0;

    int state = 0;
    int pcount = 0;
    bool finished = false;

    flag = true;
    while(!finished) {
        switch(state) {
            case 0:
                TRACE("state: %d, stack: %d, queue: %d", state, len_link_list(stack), len_link_list(queue));
                // entry point; make sure it's an expression
                if(NULL != (nterm = (ast_node*)parse_operator())) {
                    switch(token_type(((ast_operator*)nterm)->tok)) {
                        case TOK_CPAREN:
                            state = 101; // empty expression
                            break;
                        case TOK_NOT:
                        case TOK_UNARY_MINUS:
                        case TOK_OPAREN:
                            state = 3;
                            break;
                        default:
                            show_syntax("the %s operator cannot be unary", tok_to_str(((ast_operator*)nterm)->tok));
                            state = 102;
                    }
                }
                else if (NULL != (nterm = (ast_node*)parse_expr_primary()))
                    state = 8;
                else
                    state = 101;
                break;

            case 1:
                TRACE("state: %d, stack: %d, queue: %d", state, len_link_list(stack), len_link_list(queue));
                // must have a primary or a unary
                if(NULL != (nterm = (ast_node*)parse_operator()))
                    state = 3;
                else if (NULL != (nterm = (ast_node*)parse_expr_primary()))
                    state = 8;
                else
                    state = 100;
                break;

            case 2:
            case 3: {
                TRACE("state: %d, stack: %d, queue: %d", state, len_link_list(stack), len_link_list(queue));
                    // handle an operator
                    TokenType type = token_type(((ast_operator*)nterm)->tok);
                    switch(type) {
                        case TOK_OPAREN:
                            advance_token();
                            state = 4;
                            pcount++;
                            break;
                        case TOK_CPAREN:
                            pcount--;
                            if(pcount >= 0) {
                                advance_token();
                                state = 5;
                            }
                            else if(pcount < 0)
                                state = 100;
                            //state = 5;
                            break;
                        default:
                            advance_token();
                            if(get_assoc(type))
                                state = 6;
                            else
                                state = 7;
                            break;
                    }

                    expr_type = get_oper_type(type, expr_type);
                }
                break;

            case 4:
                // operator is an open paren
                push_link_list(stack, nterm);
                state = 1;
                break;

            case 5:
                TRACE("state: %d, stack: %d, queue: %d", state, len_link_list(stack), len_link_list(queue));
                // operator is close paren
                state = 1;
                while(true) {
                    ast_operator* op = (ast_operator*)pop_link_list(stack);
                    if(NULL == op) {
                        show_syntax("imbalanced parens in expression");
                        state = 102;
                        break;
                    }
                    else if(TOK_OPAREN == token_type(op->tok)) {
                        state = 1;
                        break;
                    }
                    else
                        append_link_list(queue, op);
                }
                break;

            case 6:
                TRACE("state: %d, stack: %d, queue: %d", state, len_link_list(stack), len_link_list(queue));
                // operator is left assoc
                TRACE("left assoc");
                while(peek_link_list(stack) != NULL &&
                        (get_prec(token_type(((ast_operator*)peek_link_list(stack))->tok))) >=
                        (get_prec(token_type(((ast_operator*)nterm)->tok)))) {

                    append_link_list(queue, pop_link_list(stack));
                }
                push_link_list(stack, nterm);
                state = 1;
                break;

            case 7:
                TRACE("state: %d, stack: %d, queue: %d", state, len_link_list(stack), len_link_list(queue));
                TRACE("right assoc");
                // operator it right assoc
                while(peek_link_list(stack) != NULL &&
                        (get_prec(token_type(((ast_operator*)peek_link_list(stack))->tok))) >
                        (get_prec(token_type(((ast_operator*)nterm)->tok)))) {

                    append_link_list(queue, pop_link_list(stack));
                }
                push_link_list(stack, nterm);
                state = 1;
                break;

            case 8:
                TRACE("state: %d, stack: %d, queue: %d", state, len_link_list(stack), len_link_list(queue));
                // handle a primary
                append_link_list(queue, nterm);
                state = 1;
                break;

            case 100:
                TRACE("state: %d, stack: %d, queue: %d, pcount: %d",
                        state, len_link_list(stack), len_link_list(queue), pcount);
                TRACE_TERM(get_token());
                if(pcount > 0) {
                    show_syntax("imbalanced parentheses in expression");
                    state = 102;
                }
                else {
                    // finished expression
                    while(peek_link_list(stack) != NULL)
                        append_link_list(queue, pop_link_list(stack));

                    node = CREATE_AST_NODE(AST_expression, ast_expression);
                    node->list = queue;
                    node->expr_type = expr_type;
                    finalize_token_queue();
                    finished = true;
                }
                break;

            case 101:
                TRACE("state: %d, stack: %d, queue: %d", state, len_link_list(stack), len_link_list(queue));
                // not an expression, not an error;
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                TRACE("state: %d", state);
                // exit on a syntax error;
                node = NULL;
                finished = true;
                break;

            default:
                TRACE("state: %d, stack: %d, queue: %d", state, len_link_list(stack), len_link_list(queue));
                // cannot happen
                fatal_error("invalid state in %s: %d", __func__, state);
                break;
        }
    }

    RETV(node);
}
