
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "memory.h"
#include "errors.h"
#include "token.h"
#include "scanner.h"

// generated file
#include "keywords.h"
/*
 * Be aware that this data structure is in sorted order.
 *
static struct _keyword {
    const char* str;
    token_type type;
} _keyword_list[] = {
    {"and", AND_TOK},
    {"bool", BOOL_TOK},
    {"boolean", BOOL_TOK},
    {"break", BREAK_TOK},
    {"case", CASE_TOK},
    {"class", CLASS_TOK},
    {"continue", CONTINUE_TOK},
    {"default", DEFAULT_TOK},
    {"dict", DICT_TOK},
    {"dictionary", DICT_TOK},
    {"do", DO_TOK},
    {"else", ELSE_TOK},
    {"eq", EQUALITY_TOK},
    {"except", EXCEPT_TOK},
    {"float", FLOAT_TOK},
    {"for", FOR_TOK},
    {"gte", GTE_TOK},
    {"gt", GREATER_TOK},
    {"if", IF_TOK},
    {"include", INCLUDE_TOK},
    {"int", INT_TOK},
    {"list", LIST_TOK},
    {"lte", LTE_TOK},
    {"lt", LESS_TOK},
    {"namespace", NAMESPACE_TOK},
    {"neq", NOT_EQUAL_TOK},
    {"not", NOT_TOK},
    {"or", OR_TOK},
    {"private", PRIVATE_TOK},
    {"priv", PRIVATE_TOK},
    {"protected", PROTECTED_TOK},
    {"public", PUBLIC_TOK},
    {"return", RETURN_TOK},
    {"signed", INT_TOK},
    {"string", STRING_TOK},
    {"switch", SWITCH_TOK},
    {"try", TRY_TOK},
    {"uint", UINT_TOK},
    {"unsigned", UINT_TOK},
    {"while", WHILE_TOK}
};

#define KW_LIST_LEN (sizeof(_keyword_list)/sizeof(struct _keyword))
*/

static token* make_error_token() {

    token* tok = create_token();

    add_string_strg(tok->str, "error");
    tok->type = ERROR_TOK;

    return tok;
}

static token_type find_keyword(const char* str) {

    int start = 0;
    int end = KW_LIST_LEN;
    token_type retv = SYMBOL_TOK;

    while(start <= end) {
        size_t mid = start + (end - start) / 2;
        int result;

        if(0 == (result = strcmp(str, _keyword_list[mid].str))) {
            retv = _keyword_list[mid].type;
            break;
        }

        if(result > 0)
            start = mid + 1;
        else
            end = mid - 1;
    }

    // returns SYMBOL_TOK if a keyword is not found.
    return retv;
}

/*
 * This is a normal word that cound be a symbol or a keyword. Whether it's a
 * symbol or keyword is found in a later step.
 */
static void do_symbol_or_keyword(token* tok) {

    do {
        add_string_char(tok->str, get_char());
        next_char();
    } while(IS_SYM(get_char()));

    tok->type = find_keyword(get_string(tok->str));

}

/*
 * Could be a FNUM, INUM, or UNUM. Only difference is formatting. Numeric value
 * of string is returned as part of the token.
 */
static void do_any_number(token* tok) {

    int state = 0;
    int finished = 0;

    tok->type = ERROR_TOK;
    while(!finished) {
        switch(state) {
            case 0: // initial state
                if(get_char() == '0') {
                    // could be a float or a unsigned
                    add_string_char(tok->str, get_char());
                    next_char();
                    state = 2;
                }
                else {
                    // is not an unsigned for sure
                    add_string_char(tok->str, get_char());
                    next_char();
                    state = 3;
                }
                break;

            case 1: // end
                finished++;
                break;

            case 2:
                if(get_char() == 'x' || get_char() == 'X') {
                    // it's definately a hex (unsigned) number
                    add_string_char(tok->str, get_char());
                    next_char();
                    state = 5;  // finish a hex number
                }
                else {
                    if(IS_DIGIT(get_char())) {
                        // the leading digit was a '0' but there are more
                        // digits to follow. Since octals are not supported,
                        // this is an illegal combination. Absorb the remaining
                        // digits.
                        syntax("malformed number; leading zeros are not allowed");
                        while(IS_DIGIT(get_char())) {
                            add_string_char(tok->str, get_char());
                            next_char();
                        }
                        state = 6;
                    }
                    else if(get_char() == '.') {
                        // it's definately a float, but we expect to see a '.' in
                        // the number, i.e. "0.123" or "0.0"
                        add_string_char(tok->str, get_char());
                        next_char();
                        state = 4;  // finish a float after the '.'
                    }
                    else {
                        // the zero is all alone in the number, which makes it
                        // a signed zero.
                        tok->type = INUM_TOK;
                        state = 1;
                    }
                }
                break;

            case 3: // finish a float up to the '.' or a signed to the end
                if(!IS_DIGIT(get_char())) {
                    int ch = get_char();
                    if(ch == '.') {
                        // finish a float with a possible mantisa
                        add_string_char(tok->str, ch);
                        next_char();
                        state = 4;  // finish a float
                    }
                    else {
                        // finished a signed number
                        tok->type = INUM_TOK;
                        state = 1;
                    }
                }
                else {
                    // add the character to the string
                    add_string_char(tok->str, get_char());
                    next_char();
                }
                break;

            case 4: // finish a float after the '.' with a possible mantisa
                if(!IS_DIGIT(get_char())) {
                    int ch = get_char();
                    if(ch == 'e' || ch == 'E') {
                        add_string_char(tok->str, ch);
                        next_char();
                        state = 7;
                    }
                    else {
                        tok->type = FNUM_TOK;
                        state = 1;
                    }
                }
                else {
                    // add the character to the string
                    add_string_char(tok->str, get_char());
                    next_char();
                }
                break;

            case 5: // finish a hex number
                if(isxdigit(get_char())) {
                    // add the character to the string
                    add_string_char(tok->str, get_char());
                    next_char();
                }
                else {
                    tok->type = UNUM_TOK;
                    state = 1;
                }
                break;

            case 6: // error state
                tok->type = ERROR_TOK;
                state = 1;
                break;

            case 7: {
                    // read the number for the mantisa. It could have leading
                    // '+' or '-' and will have only digits.
                    int ch = get_char();
                    if(ch == '+' || ch == '-' || IS_DIGIT(ch)) {
                        add_string_char(tok->str, ch);
                        next_char();
                        state = 8;
                    }
                    else {
                        syntax("malformed floating point number; mantisa has invalid characters in it");
                        state = 6;
                    }
                    break;
                }

            case 8:
                if(IS_DIGIT(get_char())) {
                    add_string_char(tok->str, get_char());
                    next_char();
                }
                else {
                    tok->type = FNUM_TOK;
                    state = 1;
                }

        }
    }
}

/*
 * When this is entered, the opening "\"" has already been read. Store the
 * string in the token, honoring escape sequences.
 */
static void get_dquote_str(token* tok) {

    do {
        next_char();
        int ch = get_char();
        if(ch == '\"') {
            tok->type = STR_TOK;
            next_char();
            break;
        }
        else if(ch == END_OF_FILE || ch == END_OF_INPUT) {
            syntax("end of file reached before end of string");
            tok->type = ERROR_TOK;
            break;
        }
        else {
            if(ch == '\\') {
                // have an escape character
                ch = next_char();
                switch(ch) {
                    case 'a':  add_string_char(tok->str, '\a'); break;
                    case 'b':  add_string_char(tok->str, '\b'); break;
                    case 'f':  add_string_char(tok->str, '\f'); break;
                    case 'n':  add_string_char(tok->str, '\n'); break;
                    case 'r':  add_string_char(tok->str, '\r'); break;
                    case 't':  add_string_char(tok->str, '\t'); break;
                    case 'v':  add_string_char(tok->str, '\v'); break;
                    case 'e':  add_string_char(tok->str, '\x1b'); break;
                    case '\\': add_string_char(tok->str, '\\'); break;
                    case '\'': add_string_char(tok->str, '\''); break;
                    case '\"': add_string_char(tok->str, '\"'); break;
                    case 'x': {
                            // hex number < 0xFF
                            char buf[3];
                            buf[0] = next_char();
                            buf[1] = next_char();
                            buf[2] = '\0';
                            add_string_char(tok->str, (int)strtol(buf, NULL, 16));
                        }
                        break;
                    // escaped default to the character they are escaping.
                    default:   add_string_char(tok->str, ch); break;

                }
            }
            else
                add_string_char(tok->str, ch);
        }
    } while(1);
}

/*
 * When this is entered, the opening "\'" has already been read. Store the
 * string as an absolute literal.
 */
static void get_squote_str(token* tok) {

    do {
        next_char();
        int ch = get_char();
        if(ch == '\'') {
            tok->type = STR_TOK;
            next_char();
            break;
        }
        else if(ch == END_OF_FILE || ch == END_OF_INPUT) {
            syntax("end of file reached before end of string");
            tok->type = ERROR_TOK;
            break;
        }
        else {
            add_string_char(tok->str, ch);
        }
    } while(1);
}

/*
 * Could be any operator or syntax character. Could also be a symbol if the
 * first character is [_!@$^~]. The only invalid character is a '\`'. All
 * characters that are not part of an operator are okay in a symbol.
 */
static void do_any_punct(token* tok) {

    int ch = get_char();
    switch(ch) {
        case '(':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = OPAREN_TOK;
            break;
        case ')':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = CPAREN_TOK;
            break;
        case '[':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = OSQUARE_TOK;
            break;
        case ']':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = CSQUARE_TOK;
            break;
        case '{':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = OCURLY_TOK;
            break;
        case '}':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = CCURLY_TOK;
            break;
        case '+':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = PLUS_TOK;
            break;
        case '-':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = MINUS_TOK;
            break;
        case '%':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = PERCENT_TOK;
            break;
        case '*':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = STAR_TOK;
            break;
        case '&':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = AND_TOK;
            break;
        case '|':
            add_string_char(tok->str, ch);
            next_char();
            tok->type = OR_TOK;
            break;
        case '/':
            // may have a comment or a divide operator.
            add_string_char(tok->str, ch);
            next_char();
            tok->type = SLASH_TOK;
            break;
        case ',':
            // may have a comment or a divide operator.
            add_string_char(tok->str, ch);
            next_char();
            tok->type = COMMA_TOK;
            break;
        case '.':
            // may have a comment or a divide operator.
            add_string_char(tok->str, ch);
            next_char();
            tok->type = DOT_TOK;
            break;
        case '=':
            add_string_char(tok->str, ch);
            next_char();
            if(get_char() == '=') {
                add_string_char(tok->str, get_char());
                next_char();
                tok->type = EQUALITY_TOK;
            }
            else
                tok->type = EQUAL_TOK;
            break;
        case '!':
            add_string_char(tok->str, ch);
            next_char();
            if(get_char() == '=') {
                add_string_char(tok->str, get_char());
                next_char();
                tok->type = NOT_EQUAL_TOK;
            }
            else
                tok->type = NOT_TOK;
            break;
        case '>':
            add_string_char(tok->str, ch);
            next_char();
            if(get_char() == '=') {
                add_string_char(tok->str, get_char());
                next_char();
                tok->type = GTE_TOK;
            }
            else
                tok->type = GREATER_TOK;
            break;
        case '<':
            add_string_char(tok->str, ch);
            next_char();
            if(get_char() == '=') {
                add_string_char(tok->str, get_char());
                next_char();
                tok->type = LTE_TOK;
            }
            else
                tok->type = LESS_TOK;
            break;

        case '\"':
            // collect a double quote string
            get_dquote_str(tok);
            break;

        case '\'':
            // collect a single quote string
            get_squote_str(tok);
            break;
    }

}

/*
 * Simply account for whitespace and one character tokens. Skip comments.
 *
 * Allocate memory for the new token string.
 */
static void isolate_token(token* tok) {

    skip_whitespace();
    int ch = get_char();

    if(IS_SYM(ch)) {
        // could be a symbol or keyword
        do_symbol_or_keyword(tok);
    }
    else if(IS_DIGIT(ch)) {
        // is a number of some kind
        do_any_number(tok);
    }
    else if(IS_PUNCT(ch)) {
        // is punctuation; could be a lot of things.
        do_any_punct(tok);
    }
    else if(ch == END_OF_INPUT) {
        tok->type = INPUT_END_TOK;
    }
    else if(ch == END_OF_FILE) {
        tok->type = FILE_END_TOK;
    }
    else {
        tok->type = ERROR_TOK;
    }
}

/*
 * Deliver the next token to the caller. The token is removed from the input
 * stream when this is called.
 */
token* scan_token() {

    token* tok = create_token();

    isolate_token(tok);

    return tok;
}

/*
 * Create a new token.
 */
token* create_token() {

    token* tok = _alloc(sizeof(token));
    tok->str = create_string();

    return tok;
}

/*
 * Destroy a token. This must be called for every token to avoid memory leaks.
 */
void destroy_token(token* tok) {

    destroy_string(tok->str);
    _free(tok);
}

/*
 * Scan for a token and check if it is the one specified by the token type. If
 * not then a syntax error is generated and the error token is returned. If the
 * token type is a match, then return the token.
 */
token* expect_token(token_type tok_type) {

    token* tok = scan_token();
    if(tok->type != tok_type) {
        syntax("expected %s but got %s", tok_to_str(tok_type), tok_to_str(tok->type));
        destroy_token(tok);
        return make_error_token();
    }

    return tok;
}

/*
 * Convert token type to string for error handling.
 */
const char* tok_to_str(token_type tok) {

    return (
        (tok == ERROR_TOK)? "error": \
        (tok == FILE_END_TOK)? "end of file": \
        (tok == INPUT_END_TOK)? "end of input": \
        (tok == SYMBOL_TOK)? "symbol": \
        (tok == FNUM_TOK)? "floating point number": \
        (tok == INUM_TOK)? "signed number": \
        (tok == UNUM_TOK)? "unsigned number": \
        (tok == STR_TOK)? "quoted string": \
        (tok == CLASS_TOK)? "class": \
        (tok == NAMESPACE_TOK)? "namespace": \
        (tok == INCLUDE_TOK)? "include": \
        (tok == PUBLIC_TOK)? "public": \
        (tok == PRIVATE_TOK)? "private": \
        (tok == PROTECTED_TOK)? "protected": \
        (tok == FLOAT_TOK)? "float": \
        (tok == INT_TOK)? "signed": \
        (tok == UINT_TOK)? "unsigned": \
        (tok == BOOL_TOK)? "boolean": \
        (tok == DICT_TOK)? "dictionary": \
        (tok == LIST_TOK)? "list": \
        (tok == STRING_TOK)? "string": \
        (tok == FOR_TOK)? "for": \
        (tok == WHILE_TOK)? "while": \
        (tok == DO_TOK)? "do": \
        (tok == IF_TOK)? "if": \
        (tok == ELSE_TOK)? "else": \
        (tok == SWITCH_TOK)? "switch": \
        (tok == CASE_TOK)? "case": \
        (tok == DEFAULT_TOK)? "default": \
        (tok == CONTINUE_TOK)? "continue": \
        (tok == BREAK_TOK)? "break": \
        (tok == RETURN_TOK)? "return": \
        (tok == TRY_TOK)? "try": \
        (tok == EXCEPT_TOK)? "except": \
        (tok == PLUS_TOK)? "\"add\"": \
        (tok == MINUS_TOK)? "\"subtract\"": \
        (tok == SLASH_TOK)? "\"divide\"": \
        (tok == STAR_TOK)? "\"multiply\"": \
        (tok == EQUAL_TOK)? "\"equals\"": \
        (tok == PERCENT_TOK)? "\"modulo\"": \
        (tok == EQUALITY_TOK)? "\"is equal to\"": \
        (tok == NOT_EQUAL_TOK)? "\"is not equal to\"": \
        (tok == LTE_TOK)? "\"is less or equal to\"": \
        (tok == GTE_TOK)? "\"is more or equal to\"": \
        (tok == GREATER_TOK)? "\"is greater than\"": \
        (tok == LESS_TOK)? "\"is less than\"": \
        (tok == OR_TOK)? "\"or\"": \
        (tok == AND_TOK)? "\"and\"": \
        (tok == NOT_TOK)? "\"not\"": \
        (tok == OPAREN_TOK)? "open paren": \
        (tok == CPAREN_TOK)? "close paren": \
        (tok == OCURLY_TOK)? "open curly brace": \
        (tok == CCURLY_TOK)? "close curly brace": \
        (tok == OSQUARE_TOK)? "open square brace": \
        (tok == CSQUARE_TOK)? "close square brace": \
        (tok == DOT_TOK)? "period": \
        (tok == COMMA_TOK)? "comma": "unknown"
    );
}
