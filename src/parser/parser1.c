/**
 * @file parser1.c
 *
 * @brief This is the bottom most level where the most simple objects are
 * parsed that are used many places.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-13-2024
 * @copyright Copyright (c) 2024
 */
#include "util.h"
#include "scanner.h"
#include "parser.h"

/**
 * @brief This looks for a single keyword and returns the token associated
 * with it. This can only be used in conjunction with other rules, since it
 * can only return a terminal. Therefore, it cannot return an error.
 *  TOK_PUBLIC,
 *  TOK_PRIVATE,
 *  TOK_PROTECTED,
 *
 * @return NonTerm*
 *
 */
NonTerm* scope_operator() {

    Token* tok = get_token();
    if(tok->type == TOK_PUBLIC ||
            tok->type == TOK_PRIVATE ||
            tok->type == TOK_PROTECTED) {

        NonTerm* nt = _ALLOC_T(NonTerm);
        nt->type = NTT_SCOPE_OPERATOR;
        nt->data = _ALLOC_T(ScopeOperator);
        nt->data->tok = copy_token(tok);
        advance_token();
        return nt;
    }
    else {
        reset_token_queue();
        return NULL;
    }
}

/**
 * @brief Single keyword that represents the name of a built-in type.
 *  TOK_NUMBER,
 *  TOK_NOTHING,
 *  TOK_STRING,
 *  TOK_BOOLEAN,
 *  TOK_LIST,
 *  TOK_DICT,
 *
 * @return NonTerm*
 *
 */
NonTerm* type_name() {

    Token* tok = get_token();
    if(tok->type == TOK_NUMBER ||
            tok->type == TOK_STRING ||
            tok->type == TOK_NOTHING ||
            tok->type == TOK_BOOLEAN ||
            tok->type == TOK_LIST ||
            tok->type == TOK_DICT) {

        NonTerm* nt = _ALLOC_T(NonTerm);
        nt->type = NTT_TYPE_NAME;
        nt->data = copy_token(tok);
        advance_token();
        return nt;
    }
    else {
        reset_token_queue();
        return NULL;
    }
}

/**
 * @brief Parse a compound name. A compound name has the format of
 * SYMBOL ('.' SYMBOL)*
 *
 * @return NonTerm*
 *
 */
NonTerm* compound_name() {

    Token* tok = get_token();

    if(tok->type == TOK_SYMBOL) {
        StrList* lst = create_string_list();
        add_string_list(lst, tok->str);
        tok = advance_token();
        if(tok->type == TOK_DOT) {
            // more than one element
            while(true) {
                tok = advance_token();
                if(tok->type == TOK_SYMBOL) {
                    // another element to add
                    add_string_list(lst, tok->str);
                }
                else {
                    // handle an error because a dot must be followed by
                    // a TOK_SYMBOL in this context.
                    handle_error("expected a symbol but got a %s", raw_string(tok->str));
                    return NULL;
                }

                tok = advance_token();
                if(tok->type != TOK_DOT) {
                    // non-terminal is complete
                    NonTerm* nt = _ALLOC_T(NonTerm);
                    nt->type = NTT_COMPOUND_NAME;
                    nt->data = lst;
                    advance_token();
                    return nt;
                }
            }
        }
        else {
            // single symbol is a match
            NonTerm* nt = _ALLOC_T(NonTerm);
            nt->type = NTT_COMPOUND_NAME;
            nt->data = lst;
            advance_token();
            return nt;
        }
    }
    else {
        // not a match
        reset_token_queue();
        return NULL;
    }
}

/**
 * @brief A type spec element is a component of a complete type specification.
 *      type_spec_element
 *          : type_name
 *          | compound_name
 *
 * @return NonTerm*
 *
 */
NonTerm* type_spec_element() {

    NonTerm* nt;

    if(NULL != (nt = type_name())) {
        // the element is a valid type name
        NonTerm* nterm = _ALLOC_T(NonTerm);
        nterm->type = NTT_TYPE_SPEC_ELEMENT;
        nterm->data = nt;
        advance_token();
        return nterm;
    }
    else {
        // this is not a match, but it could match the next alternative.
        reset_token_queue();
    }

    if(NULL != (nt = compound_name())) {
        // the element is a valid compound name
        NonTerm* nterm = _ALLOC_T(NonTerm);
        nterm->type = NTT_TYPE_SPEC_ELEMENT;
        nterm->data = nt;
        advance_token();
        return nterm;
    }
    else {
        // this is not a match either, but this does not generate an error.
        reset_token_queue();
    }

    // there is no match, but that is not an error, yet.
    return NULL;
}

/**
 * @brief A type spec is a complete specification to name a type.
 *      type_spec
 *          : CONST type_spec_element
 *          | type_spec_element
 *
 * @return NonTerm*
 *
 */
NonTerm* type_spec() {

    NonTerm* nt;

    if(NULL == ())
}
