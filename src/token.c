
#include <stdarg.h> // see validate_token()
#include "common.h"

// generated file
#include "keywords.h"

static token* current_token = NULL;

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
 * Split a string with '.'s in it into an array of strings in left to right
 * order. The string list is returned in the obj data element of the token as
 * a str_list_t*.
 */
void split_compound(token* tok, const char* str) {

    str_list_t* lst = create_str_list();
    char* buf = (char*)_strdup(str);

    for(char* tmp = strtok(buf, "."); tmp != NULL; tmp = strtok(NULL, "."))
        add_str_list(lst, tmp);

    tok->data.obj = lst->list;
    _free(buf);
    // don't destroy it because that would free the list.
    _free(lst);
}

/*
 * This is a normal word that could be a symbol or a keyword. Whether it's a
 * symbol or keyword is found in a later step.
 */
static void do_symbol_or_keyword(token* tok) {

    string* str = create_string();
    int flag = 0;

    do {
        add_string_char(str, get_char());
        next_char();
        if(get_char() == '.')
            flag++;
    } while(IS_SYM(get_char()));

    if(flag) {
        tok->type = COMPOUND_NAME_TOK;
        split_compound(tok, str->str);
    }
    else
        tok->type = find_keyword(str->str);
    tok->str = _strdup(str->str);
    destroy_string(str);

}

/*
 * Could be a FNUM, INUM, or UNUM. Only difference is formatting. Numeric value
 * of string is returned as part of the token.
 */
static void do_any_number(token* tok) {

    int state = 0;
    int finished = 0;
    string* str = create_string();

    tok->type = ERROR_TOK;
    while(!finished) {
        switch(state) {
            case 0: // initial state
                if(get_char() == '0') {
                    // could be a float or a unsigned
                    add_string_char(str, get_char());
                    next_char();
                    state = 2;
                }
                else {
                    // is not an unsigned for sure
                    add_string_char(str, get_char());
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
                    add_string_char(str, get_char());
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
                            add_string_char(str, get_char());
                            next_char();
                        }
                        state = 6;
                    }
                    else if(get_char() == '.') {
                        // it's definately a float, but we expect to see a '.' in
                        // the number, i.e. "0.123" or "0.0"
                        add_string_char(str, get_char());
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
                        add_string_char(str, ch);
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
                    add_string_char(str, get_char());
                    next_char();
                }
                break;

            case 4: // finish a float after the '.' with a possible mantisa
                if(!IS_DIGIT(get_char())) {
                    int ch = get_char();
                    if(ch == 'e' || ch == 'E') {
                        add_string_char(str, ch);
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
                    add_string_char(str, get_char());
                    next_char();
                }
                break;

            case 5: // finish a hex number
                if(isxdigit(get_char())) {
                    // add the character to the string
                    add_string_char(str, get_char());
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
                        add_string_char(str, ch);
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
                    add_string_char(str, get_char());
                    next_char();
                }
                else {
                    tok->type = FNUM_TOK;
                    state = 1;
                }

        }
    }

    tok->str = _strdup(str->str);
    destroy_string(str);
}

/*
 * When this is entered, the opening "\"" has already been read. Store the
 * string in the token, honoring escape sequences.
 */
static void get_dquote_str(token* tok) {

    string* str = create_string();

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
                    case 'a':  add_string_char(str, '\a'); break;
                    case 'b':  add_string_char(str, '\b'); break;
                    case 'f':  add_string_char(str, '\f'); break;
                    case 'n':  add_string_char(str, '\n'); break;
                    case 'r':  add_string_char(str, '\r'); break;
                    case 't':  add_string_char(str, '\t'); break;
                    case 'v':  add_string_char(str, '\v'); break;
                    case 'e':  add_string_char(str, '\x1b'); break;
                    case '\\': add_string_char(str, '\\'); break;
                    case '\'': add_string_char(str, '\''); break;
                    case '\"': add_string_char(str, '\"'); break;
                    case 'x': {
                            // hex number < 0xFF
                            char buf[3];
                            buf[0] = next_char();
                            buf[1] = next_char();
                            buf[2] = '\0';
                            add_string_char(str, (int)strtol(buf, NULL, 16));
                        }
                        break;
                    // escaped default to the character they are escaping.
                    default:   add_string_char(str, ch); break;

                }
            }
            else
                add_string_char(str, ch);
        }
    } while(1);

    tok->str = _strdup(str->str);
    destroy_string(str);

}

/*
 * When this is entered, the opening "\'" has already been read. Store the
 * string as an absolute literal.
 */
static void get_squote_str(token* tok) {

    string* str = create_string();

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
            add_string_char(str, ch);
        }
    } while(1);

    tok->str = _strdup(str->str);
    destroy_string(str);
}

/*
 * Could be any operator or syntax character. Could also be a symbol if the
 * first character is [_!@$^~]. The only invalid character is a '\`'. All
 * characters that are not part of an operator are okay in a symbol.
 */
static void do_any_punct(token* tok) {

    string* str = create_string();

    int ch = get_char();
    switch(ch) {
        case '(':
            add_string_char(str, ch);
            next_char();
            tok->type = OPAREN_TOK;
            break;
        case ')':
            add_string_char(str, ch);
            next_char();
            tok->type = CPAREN_TOK;
            break;
        case '[':
            add_string_char(str, ch);
            next_char();
            tok->type = OSQUARE_TOK;
            break;
        case ']':
            add_string_char(str, ch);
            next_char();
            tok->type = CSQUARE_TOK;
            break;
        case '{':
            add_string_char(str, ch);
            next_char();
            tok->type = OCURLY_TOK;
            break;
        case '}':
            add_string_char(str, ch);
            next_char();
            tok->type = CCURLY_TOK;
            break;
        case '+':
            add_string_char(str, ch);
            next_char();
            tok->type = PLUS_TOK;
            break;
        case '-':
            add_string_char(str, ch);
            next_char();
            tok->type = MINUS_TOK;
            break;
        case '%':
            add_string_char(str, ch);
            next_char();
            tok->type = PERCENT_TOK;
            break;
        case '*':
            add_string_char(str, ch);
            next_char();
            tok->type = STAR_TOK;
            break;
        case '&':
            add_string_char(str, ch);
            next_char();
            tok->type = AND_TOK;
            break;
        case '|':
            add_string_char(str, ch);
            next_char();
            tok->type = OR_TOK;
            break;
        case '/':
            // may have a comment or a divide operator.
            add_string_char(str, ch);
            next_char();
            tok->type = SLASH_TOK;
            break;
        case ',':
            // may have a comment or a divide operator.
            add_string_char(str, ch);
            next_char();
            tok->type = COMMA_TOK;
            break;
        case '.':
            // may have a comment or a divide operator.
            add_string_char(str, ch);
            next_char();
            tok->type = DOT_TOK;
            break;
        case '=':
            add_string_char(str, ch);
            next_char();
            if(get_char() == '=') {
                add_string_char(str, get_char());
                next_char();
                tok->type = EQUALITY_TOK;
            }
            else
                tok->type = EQUAL_TOK;
            break;
        case '!':
            add_string_char(str, ch);
            next_char();
            if(get_char() == '=') {
                add_string_char(str, get_char());
                next_char();
                tok->type = NOT_EQUAL_TOK;
            }
            else
                tok->type = NOT_TOK;
            break;
        case '>':
            add_string_char(str, ch);
            next_char();
            if(get_char() == '=') {
                add_string_char(str, get_char());
                next_char();
                tok->type = GTE_TOK;
            }
            else
                tok->type = GREATER_TOK;
            break;
        case '<':
            add_string_char(str, ch);
            next_char();
            if(get_char() == '=') {
                add_string_char(str, get_char());
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
    tok->str = _strdup(str->str);
    destroy_string(str);

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
 * Create a new token.
 */
static token* create_token() {

    token* tok = _alloc(sizeof(token));
    tok->str = NULL; //create_string();
    tok->type = ERROR_TOK;

    return tok;
}

/*
 * Deliver the next token to the caller. The token is removed from the input
 * stream when this is called.
 */
static token* scan_token() {

    token* tok = create_token();

    isolate_token(tok);

    return tok;
}

/*
 * Destroy a token. This must be called for every token to avoid memory leaks.
 */
static void destroy_token(token* tok) {

    _free((void*)tok->str);
    _free(tok);
}

/*************************************
 * Public interface
 */

/*
 * Validate a token against a list of token types. Publish a syntax error if
 * the given token does not match one of the parameters. Automatically calls
 * recover..()
 */
bool validate_token(token* tok, int nargs, ...) {

    va_list args;

    va_start(args, nargs);
    token_type type = tok->type;
    for(int i = 0; i < nargs; i++) {
        if(type == va_arg(args, token_type))
            return true; // yes, token is valid
    }

    // if we reach here then the token is not a match. Build the string and
    // publish the syntax error.
    fprintf(stderr, "error: %s: %d: %d: ", get_fname(), get_line_no(), get_col_no());
    fprintf(stderr, "expected ");

    va_start(args, nargs);
    for(int i = 0; i < nargs; i++) {
        fprintf(stderr, "%s", tok_to_str(va_arg(args, token_type)));
        if(i + 1 < nargs)
            fprintf(stderr, ", ");
        if(i + 2 == nargs)
            fprintf(stderr, "or ");

    }
    va_end(args);

    fprintf(stderr, " but got ");
    fprintf(stderr, "%s\n", tok_to_str(tok->type));

    recover_from_error();
    bump_error_count();
    return false;   // no, token did not validate
}


/*
 * Simply return the current token in the stream.
 */
token* get_token() {

    return current_token;
}

/*
 * Advanse to the next token in the stream.
 */
token* next_token() {

    if(current_token != NULL)
        destroy_token(current_token);

    current_token = scan_token();
    return current_token;
}

/*
 * Convert token type to string for error handling.
 */
const char* tok_to_str(token_type tok) {

    return (
        (tok == ERROR_TOK)? "error":
        (tok == FILE_END_TOK)? "end of file":
        (tok == INPUT_END_TOK)? "end of input":
        (tok == SYMBOL_TOK)? "symbol":
        (tok == FNUM_TOK)? "floating point number":
        (tok == INUM_TOK)? "signed number":
        (tok == UNUM_TOK)? "unsigned number":
        (tok == STR_TOK)? "quoted string":
        (tok == CLASS_TOK)? "class keyword":
        (tok == IMPORT_TOK)? "import keyword":
        (tok == PUBLIC_TOK)? "public keyword":
        (tok == PRIVATE_TOK)? "private keyword":
        (tok == PROTECTED_TOK)? "protected keyword":
        (tok == FLOAT_TOK)? "float keyword":
        (tok == INT_TOK)? "signed (or int) keyword":
        (tok == UINT_TOK)? "unsigned (or uint) keyword":
        (tok == BOOL_TOK)? "boolean (or bool) keyword":
        (tok == DICT_TOK)? "dictionary (or dict) keyword":
        (tok == LIST_TOK)? "list keyword":
        (tok == STRING_TOK)? "string keyword":
        (tok == FOR_TOK)? "for keyword":
        (tok == WHILE_TOK)? "while keyword":
        (tok == DO_TOK)? "do keyword":
        (tok == IF_TOK)? "if keyword":
        (tok == ELSE_TOK)? "else keyword":
        (tok == SWITCH_TOK)? "switch keyword":
        (tok == CASE_TOK)? "case keyword":
        (tok == DEFAULT_TOK)? "default keyword":
        (tok == CONTINUE_TOK)? "continue keyword":
        (tok == BREAK_TOK)? "break keyword":
        (tok == RETURN_TOK)? "return keyword":
        (tok == TRY_TOK)? "try keyword":
        (tok == EXCEPT_TOK)? "except keyword":
        (tok == ENTER_TOK)? "enter keyword":
        (tok == LEAVE_TOK)? "leave keyword":
        (tok == ANY_TOK)? "any (or nothing) keyword":
        (tok == PLUS_TOK)? "\"add\" (+)":
        (tok == MINUS_TOK)? "\"subtract\" (-)":
        (tok == SLASH_TOK)? "\"divide\" (/)":
        (tok == STAR_TOK)? "\"multiply\" (*)":
        (tok == EQUAL_TOK)? "\"equals\" (=)":
        (tok == PERCENT_TOK)? "\"modulo\" (%)":
        (tok == EQUALITY_TOK)? "\"is equal to\" (eq | ==)":
        (tok == NOT_EQUAL_TOK)? "\"is not equal to\" (neq | !=)":
        (tok == LTE_TOK)? "\"is less or equal to\" (lte | <=)":
        (tok == GTE_TOK)? "\"is more or equal to\" (gte | >=)":
        (tok == GREATER_TOK)? "\"is greater than\" (gt | >)":
        (tok == LESS_TOK)? "\"is less than\" (lt | <)":
        (tok == OR_TOK)? "\"or\" (or | |)":
        (tok == AND_TOK)? "\"and\" (and | &)":
        (tok == NOT_TOK)? "\"not\" (not | !)":
        (tok == OPAREN_TOK)? "open paren ('(')":
        (tok == CPAREN_TOK)? "close paren (')')":
        (tok == OCURLY_TOK)? "open curly brace ('{')":
        (tok == CCURLY_TOK)? "close curly brace ('}')":
        (tok == OSQUARE_TOK)? "open square brace ('[')":
        (tok == CSQUARE_TOK)? "close square brace (']')":
        (tok == DOT_TOK)? "period ('.')":
        (tok == COMMA_TOK)? "comma (',')":
        (tok == COMPOUND_NAME_TOK)? "compound name": "unknown"
    );
}
