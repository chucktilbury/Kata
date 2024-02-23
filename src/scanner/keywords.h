
/* This file is generated with a script. Do not edit. */
#ifndef _KEYWORD_H
#define _KEYWORD_H
#include "scanner.h"

typedef enum {
    // markers
    TOK_END_OF_FILE,
    TOK_END_OF_INPUT,
    TOK_ERROR,

    // keywords
    TOK_AND,
    TOK_AS,
    TOK_BOOL,
    TOK_BOOLEAN,
    TOK_BREAK,
    TOK_CASE,
    TOK_CLASS,
    TOK_CONST,
    TOK_CONTINUE,
    TOK_CREATE,
    TOK_DEFAULT,
    TOK_DESTROY,
    TOK_DICT,
    TOK_DO,
    TOK_ELSE,
    TOK_EXCEPT,
    TOK_EXIT,
    TOK_FALSE,
    TOK_FLOAT,
    TOK_FOR,
    TOK_IF,
    TOK_IMPORT,
    TOK_IN,
    TOK_INLINE,
    TOK_LIST,
    TOK_NAMESPACE,
    TOK_NOT,
    TOK_NOTH,
    TOK_NOTHING,
    TOK_OFF,
    TOK_ON,
    TOK_OR,
    TOK_PRINT,
    TOK_PRIVATE,
    TOK_PROTECTED,
    TOK_PUBLIC,
    TOK_RAISE,
    TOK_RETURN,
    TOK_SIGNED,
    TOK_START,
    TOK_STRG,
    TOK_STRING,
    TOK_STRUCT,
    TOK_SWITCH,
    TOK_TO,
    TOK_TRACE,
    TOK_TRUE,
    TOK_TRY,
    TOK_TYPE,
    TOK_UNSIGNED,
    TOK_VIRTUAL,
    TOK_WHILE,
    TOK_YIELD,


    // operators
    TOK_LORE,
    TOK_GORE,
    TOK_EQU,
    TOK_NEQU,
    TOK_OR,
    TOK_AND,
    TOK_NOT,
    TOK_ADD_ASSIGN,
    TOK_SUB_ASSIGN,
    TOK_MUL_ASSIGN,
    TOK_DIV_ASSIGN,
    TOK_MOD_ASSIGN,
    TOK_ADD,
    TOK_SUB,
    TOK_ASSIGN,
    TOK_DIV,
    TOK_MUL,
    TOK_MOD,
    TOK_OPAREN,
    TOK_CPAREN,
    TOK_OCBRACE,
    TOK_CCBRACE,
    TOK_OSBRACE,
    TOK_CSBRACE,
    TOK_COMMA,
    TOK_DOT,
    TOK_OPBRACE,
    TOK_CPBRACE,
    TOK_COLON,
    TOK_CARAT,
    TOK_AMPER,

    // constructed tokens
    TOK_LITERAL_FLOAT,
    TOK_LITERAL_DSTR,
    TOK_LITERAL_SSTR,
    TOK_SYMBOL,
    TOK_UNARY_MINUS,
} TokenType;

typedef struct {
    const char* str;
    TokenType type;
} KeywordList;

extern KeywordList keyword_list[];
extern const int num_keywords;

const char* tok_to_str(Token* tok);

#endif /* _KEYWORDS_H */

