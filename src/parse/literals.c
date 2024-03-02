/**
 * @file literals.c
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
 *  scope_operator
 *      = 'private'
 *      / 'public'
 *      / 'protected'
 *
 * @return ast_scope_operator*
 *
 */
ast_scope_operator* parse_scope_operator() {

    ENTER;
    ast_scope_operator* node = NULL;
    TokenType ttype = token_type(get_token());

    if((TOK_PRIVATE == ttype) ||
            (TOK_PUBLIC == ttype) ||
            (TOK_PROTECTED == ttype)) {

        TRACE_TERM(get_token());
        node = CREATE_AST_NODE(AST_scope_operator, ast_scope_operator);
        node->token = get_token();
        set_scope((ttype == TOK_PRIVATE)? SCOPE_PRIV:
                    (ttype == TOK_PUBLIC)? SCOPE_PUB:
                    (ttype == TOK_PROTECTED)? SCOPE_PROT: SCOPE_PRIV);
        finalize_token();
        advance_token();
    }

    RETV(node);
}

/**
 * @brief
 *
 *  literal_type_name
 *      = 'float'
 *      / 'unsigned'
 *      / 'signed'
 *      / 'string'
 *      / 'boolean'
 *      / 'nothing'
 *      / 'list'
 *      / 'dict'
 *      / 'function'
 *
 * @return ast_literal_type_name*
 *
 */
ast_literal_type_name* parse_literal_type_name() {

    ENTER;
    ast_literal_type_name* node = NULL;
    TokenType ttype = token_type(get_token());

    if((TOK_FLOAT == ttype) ||
            (TOK_UNSIGNED == ttype) ||
            (TOK_SIGNED == ttype) ||
            (TOK_STRING == ttype) ||
            (TOK_BOOLEAN == ttype) ||
            (TOK_NOTHING == ttype) ||
            (TOK_LIST == ttype) ||
            (TOK_DICT == ttype) ||
            (TOK_FUNCTION == ttype)) {

        TRACE_TERM(get_token());
        node = CREATE_AST_NODE(AST_literal_type_name, ast_literal_type_name);
        node->token = get_token();
        finalize_token();
        advance_token();
    }

    RETV(node);
}

/**
 * @brief
 *
 *  literal_value
 *      = LITERAL_FLOAT
 *      / LITERAL_UNSIGNED
 *      / LITERAL_SIGNED
 *      / LITERAL_BOOL
 *
 * @return ast_literal_value*
 *
 */
ast_literal_value* parse_literal_value() {

    ENTER;
    ast_literal_value* node = NULL;
    TokenType ttype = token_type(get_token());

    if((TOK_LITERAL_FLOAT == ttype) ||
            (TOK_LITERAL_UNSIGNED == ttype) ||
            (TOK_LITERAL_SIGNED == ttype) ||
            (TOK_TRUE == ttype) ||
            (TOK_FALSE == ttype) ||
            (TOK_ON == ttype) ||
            (TOK_OFF == ttype)) {

        TRACE_TERM(get_token());
        node = CREATE_AST_NODE(AST_literal_value, ast_literal_value);
        node->token = get_token();
        finalize_token();
        advance_token();

        switch(ttype) {
            case TOK_LITERAL_FLOAT:
                node->value.fnum = strtod(raw_string(get_token()->str), NULL);
                break;
            case TOK_LITERAL_SIGNED:
                node->value.snum = strtol(raw_string(get_token()->str), NULL, 10);
                break;
            case TOK_LITERAL_UNSIGNED:
                node->value.unum = strtol(raw_string(get_token()->str), NULL, 16);
                break;
            case TOK_TRUE:
            case TOK_ON:
                node->value.bval = true;
                break;
            case TOK_FALSE:
            case TOK_OFF:
                node->value.bval = false;
                break;
            default:
                // never can happen
                fatal_error("invalid token type in %s(): %d", __func__, ttype);
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  type_name
 *      = literal_type_name
 *      / compound_name
 *
 * @return ast_type_name*
 *
 */
ast_type_name* parse_type_name() {

    ENTER;
    ast_type_name* node = NULL;
    ast_node* nterm;

    if((NULL != (nterm = (ast_node*)parse_literal_type_name())) ||
            (NULL != (nterm = (ast_node*)parse_compound_name()))) {

        TRACE_TERM(get_token());
        node = CREATE_AST_NODE(AST_type_name, ast_type_name);
        node->nterm = nterm;
        finalize_token_queue();
    }

    RETV(node);
}


