/**
 * @file expression.c
 *
 * @brief This implements expressions for the parser. It parses expressions
 * using the shunting yard algorithm and returns them as a simple list of
 * tokens in the RPN order of operations.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-09-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_parser.h"

/*
 * Return true if the token is an operator. Otherwise return false.
 */
static bool is_operator(TokenType type) {

    return (type == TOK_LORE)         ? true :
            (type == TOK_GORE)        ? true :
            (type == TOK_EQU)         ? true :
            (type == TOK_NEQU)        ? true :
            (type == TOK_OR)          ? true :
            (type == TOK_AND)         ? true :
            (type == TOK_OPBRACE)     ? true :
            (type == TOK_CPBRACE)     ? true :
            (type == TOK_ADD)         ? true :
            (type == TOK_SUB)         ? true :
            (type == TOK_ASSIGN)      ? true :
            (type == TOK_DIV)         ? true :
            (type == TOK_MUL)         ? true :
            (type == TOK_MOD)         ? true :
            (type == TOK_NOT)         ? true :
            (type == TOK_CARAT)       ? true :
            (type == TOK_UNARY_MINUS) ? true :
                                        false;
}

/*
 * Return the precedence of the operator where the highest precedence is
 * the highest number.
 */
static int get_precedence(TokenType type) {

    return (type == TOK_LORE)         ? 0 :
            (type == TOK_GORE)        ? 0 :
            (type == TOK_EQU)         ? 0 :
            (type == TOK_NEQU)        ? 0 :
            (type == TOK_OR)          ? 0 :
            (type == TOK_AND)         ? 0 :
            (type == TOK_OPBRACE)     ? 0 :
            (type == TOK_CPBRACE)     ? 0 :
            (type == TOK_ADD)         ? 0 :
            (type == TOK_SUB)         ? 0 :
            (type == TOK_ASSIGN)      ? 0 :
            (type == TOK_DIV)         ? 0 :
            (type == TOK_MUL)         ? 0 :
            (type == TOK_MOD)         ? 0 :
            (type == TOK_CARAT)       ? 0 :
            (type == TOK_NOT)         ? 0 :
            (type == TOK_UNARY_MINUS) ? 0 :
                                        -1;
}

/**
 * @brief A primary expression is a simple single entity that an expression
 * can operate upon.
 *      primary_expression
 *          = LITERAL_NUMBER
 *          / formatted_string
 *          / compound_reference
 *          / cast_expression
 *          / '(' expression ')' # shunting yard does not use this.
 *
 * @return AstNode*
 *
 *  NOTE: compound reference has not been implemented yet.
 */
AstPrimaryExpression* parse_primary_expression() {

    ENTER;
    Token* tok = get_token();
    PtrList* list = create_ptr_list();
    AstPrimaryExpression* node = CREATE_AST_NODE(AST_primary_expression, AstPrimaryExpression);
    AstNode* nterm = NULL;

    if(TOK_LITERAL_FLOAT == tok->type) {
        node->num = tok;
        finalize_token();
        advance_token();
        RETV(node);
    }
    else if(NULL != (nterm = (AstNode*)parse_formatted_string())) {
        node->elem = nterm;
        RETV(node);
    }
    // else if(NULL != (nterm = (AstNode*)parse_compound_reference())) {
    //     node->elem = nterm;
    //     RETV(node);
    // }
    // else if(NULL != (nterm = (AstNode*)parse_cast_expression())) {
    //     node->elem = nterm;
    //     RETV(node);
    // }

    RETV(NULL);
}

/**
 * @brief Expression lists are used for things like parameter lists of
 * function calls or formatted strings.
 *      expression_list
 *          = '(' (expression (',' expression)*)? ')'
 *
 * The open and close parens are required. Having an expression inside the
 * parens are optional. If there is no expression, but there is a comma, then
 * that is a syntax error. If there is an expression followed by a comma and
 * then a close paren, then that is a syntax error.
 *
 * @return AstNode*
 *
 */
AstExpressionList* parse_expression_list() {

    ENTER;
    Token* tok = get_token();
    int state = 0;
    bool finished = false;

    PtrList* list = create_ptr_list();
    AstExpression* node = NULL;
    AstExpressionList* nterm = NULL;

    while(!finished) {
        switch(state) {
            case 0:
                // must have an oparen or there is no expression list
                tok = get_token();
                if(TOK_OPAREN == tok->type) {
                    finalize_token();
                    advance_token();
                    state = 1;
                }
                // not an error, no expression list present
                else
                    state = 101;
                break;

            case 1:
                // may have a cparen or an expression
                tok = get_token();
                if(TOK_CPAREN == tok->type) {
                    finalize_token();
                    advance_token();
                    state = 100; // empty parens is okay
                }
                else if(NULL != (node = parse_expression())) {
                    add_ptr_list(list, node);
                    state = 3;
                }
                else {
                    EXPECTED("an expression or ')'");
                    state = 101;
                }
                break;

            case 2:
                // must be an expression
                if(NULL != (node = parse_expression())) {
                    add_ptr_list(list, node);
                    state = 3;
                }
                else {
                    EXPECTED("an expression");
                    state = 101;
                }
                break;

            case 3:
                // must be a ')' or a ','
                tok = get_token();
                if(TOK_COMMA == tok->type) {
                    finalize_token();
                    advance_token();
                    state = 2;
                }
                else if(TOK_CPAREN == tok->type) {
                    finalize_token();
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("')' or ','");
                    state = 101;
                }
                break;

            case 100:
                // function will return pointer to node
                nterm = CREATE_AST_NODE(AST_expression_list, AstExpressionList);
                nterm->lst = list;
                finalize_token_queue();
                finished = true;
                break;

            case 101:
                // function will return NULL (using GC)
                finished = true;
                break;

            default:
                fatal_error("invalid state in %s(): %d", __func__, state);
                break;
        }
    }

    RETV(nterm);
}

/**
 * @brief The expressions are parsed according to the shunting yard algorithm,
 * using precedence of the operators as given by the table above.
 *
 * @return AstNode*
 *
 */
AstExpression* parse_expression() {

    ENTER;
    get_precedence(0);
    is_operator(0);
    RETV(NULL);
}

/**
 * @brief This is a cast statement. It is used to change the type of an
 * object.
 *      cast_statment
 *          '(' type_spec ')' expression
 *
 * @return AstNode*
 *
 */
AstCastExpression* parse_cast_expression() {

    ENTER;
    RETV(NULL);
}
