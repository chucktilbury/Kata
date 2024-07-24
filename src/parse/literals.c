/**
 * @file literals.c
 *
 * @brief Note that there is no parse_error() function.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-26-2024
 * @copyright Copyright (c) 2024
 */
#include <stdlib.h>

#include "trace.h"
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
ast_scope_operator* parse_scope_operator(void) {

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
ast_literal_type_name* parse_literal_type_name(void) {

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
        advance_token();
    }

    RETV(node);
}

// TODO: Make funciton match production
/**
 * @brief These are used in arithmetic expressions.
 *
 *  literal_value
 *      = LITERAL_FLOAT
 *      / LITERAL_UNSIGNED
 *      / LITERAL_SIGNED
 *      / LITERAL_BOOL
 *      / string_literal
 *
 * @return ast_literal_value*
 *
 */
ast_literal_value* parse_literal_value(void) {

    ENTER;
    ast_literal_value* node = NULL;
    TokenType ttype = token_type(get_token());
    ast_string_literal* str;

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
        node->str = NULL;
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
    else if(NULL != (str = parse_string_literal())) {
        TRACE("string literal");
        node = CREATE_AST_NODE(AST_literal_value, ast_literal_value);
        node->token = NULL;
        node->str = str;
    }
    // else not a match

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
ast_type_name* parse_type_name(void) {

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

/**
 * @brief
 *
 *  type_name_list
 *      = '(' (type_name (',' type_name)* )? ')'
 *
 * @return ast_type_name_list*
 *
 */
ast_type_name_list* parse_type_name_list(void) {

    ENTER;
    ast_type_name_list* node = NULL;
    ast_node* nterm;
    LinkList* list = create_link_list();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // must start with a '('
                if(TOK_OPAREN == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                // can be a type name or a ')'
                if(NULL != (nterm = (ast_node*)parse_type_name())) {
                    append_link_list(list, nterm);
                    state = 2;
                }
                else if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a type name or a ')'");
                    state = 102;
                }
                break;

            case 2:
                // must be a ',' or a ')'
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    state = 3;
                }
                else if(TOK_CPAREN == TTYPE) {
                    advance_token();
                    state = 100;
                }
                else {
                    EXPECTED("a ',' or a ')'");
                    state = 102;
                }
                break;

            case 3:
                // must be a type name or error
                if(NULL != (nterm = (ast_node*)parse_type_name())) {
                    append_link_list(list, nterm);
                    state = 2;
                }
                else {
                    EXPECTED("a type name");
                    state = 102;
                }
                break;

            case 100:
                // finished parsing non-terminal
                node = CREATE_AST_NODE(AST_type_name_list, ast_type_name_list);
                node->list = list;
                finalize_token_queue();
                finished = true;
                break;

            case 101:
                // this is not a match and not an error
                // There are no tokens consumed that need to be reset.
                finished = true;
                break;

            case 102:
                // there was an error
                node = NULL;
                finished = true;
                break;

            default:
                fatal_error("invalid state in %s: %d", __func__, state);
        }
    }

    RETV(node);
}

