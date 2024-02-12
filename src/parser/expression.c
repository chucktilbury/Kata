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
 *          / cast_statement
 *
 * @return AstNode*
 *
 *  NOTE: formatted string and compound reference hace not been implemented yet.
 */
AstNode* parse_primary_expression() {

    ENTER;
    RETV(NULL);
}

/**
 * @brief Expression lists are used for things like parameter lists of
 * function calls or formatted strings.
 *      expression_list
 *          = expression (',' expression)*
 *
 * @return AstNode*
 *
 */
AstNode* parse_expression_list() {

    ENTER;
    RETV(NULL);
}

/**
 * @brief The expressions are parsed according to the shunting yard algorithm,
 * using precedence of the operators as given by the table above.
 *
 * @return AstNode*
 *
 */
AstNode* parse_expression() {

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
AstNode* parse_cast_statement() {

    ENTER;
    RETV(NULL);
}
