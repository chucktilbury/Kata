
#ifndef TOKEN_H
#define TOKEN_H

typedef enum {

    ERROR_TOK = 0,  // returned when a scanner error is encountered.
    FILE_END_TOK,   // returned for an end of file situation.
    INPUT_END_TOK,  // returned when there is no more input to process.

    // constructed tokens
    SYMBOL_TOK = 1000,  // [A-Za-z_!@#$][0-9A-Za-z_!@#$]*
    COMPOUND_NAME_TOK,  // SYMBOL with dots in it.
    FNUM_TOK,       // ([0-9]*\.)?[0-9]+([Ee][-+]?[0-9]+)?
    INUM_TOK,       // ([1-9]+[0-9]*)|0
    UNUM_TOK,       // 0[Xx][A-Fa-f0-9]+
    STR_TOK,        // a quoted string. Strings enclosed in double quotes
    // support escape characters and string enclosed in single quotes are
    // copied without change.

    // keyword tokens
    CLASS_TOK,      // class
    IMPORT_TOK,     // import
    PUBLIC_TOK,     // public
    PRIVATE_TOK,    // private
    PROTECTED_TOK,  // protected, protect
    ENTER_TOK,      // enter
    LEAVE_TOK,      // leave
    CONSTRUCT_TOK,  // construct
    DESTROY_TOK,    // destroy

    // native type keywords
    FLOAT_TOK,      // float
    INT_TOK,        // int, signed
    UINT_TOK,       // uint, unsigned
    BOOL_TOK,       // bool, boolean
    DICT_TOK,       // dict, dictionary
    LIST_TOK,       // list
    STRING_TOK,     // string (keyword)
    ANY_TOK,        // nothing, any

    // flow keywords
    FOR_TOK,        // for
    WHILE_TOK,      // while
    DO_TOK,         // do
    IF_TOK,         // if
    ELSE_TOK,       // else
    SWITCH_TOK,     // switch
    CASE_TOK,       // case
    DEFAULT_TOK,    // default
    CONTINUE_TOK,   // continue
    BREAK_TOK,      // break
    RETURN_TOK,     // return
    TRY_TOK,        // try
    EXCEPT_TOK,     // except
    RAISE_TOK,      // raise, throw

    // arithmetic operators
    PLUS_TOK,       // +
    MINUS_TOK,      // -
    SLASH_TOK,      // /
    STAR_TOK,       // *
    EQUAL_TOK,      // =
    PERCENT_TOK,    // %

    // comparison operator tokens
    EQUALITY_TOK,   // ==, eq
    NOT_EQUAL_TOK,  // !=, neq
    LTE_TOK,        // <=, lte
    GTE_TOK,        // >=, gte
    GREATER_TOK,    // >, gt
    LESS_TOK,       // <, lt
    OR_TOK,         // |, or
    AND_TOK,        // &, and
    NOT_TOK,        // !, not

    // syntax operator tokens
    OPAREN_TOK,     // (
    CPAREN_TOK,     // )
    OCURLY_TOK,     // {
    CCURLY_TOK,     // }
    OSQUARE_TOK,    // [
    CSQUARE_TOK,    // ]
    DOT_TOK,        // .
    COMMA_TOK,      // ,

    // all other single characters are valid for symbols except quote and
    // comment characters.
} token_type;

#include "strings.h"



// Tokens.
typedef struct _token {
    const char* str;
    token_type type;
    union {
        double fnum;
        uint64_t unum;
        int64_t inum;
        bool boolean;
        void* obj;
    } data;
    //struct _scanner scanner;   // This is a snapshot of the scanner state
} token;

token* get_token();
token* next_token();
bool validate_token(token* tok, int nargs, ...);
const char* tok_to_str(token_type tok);

#endif
