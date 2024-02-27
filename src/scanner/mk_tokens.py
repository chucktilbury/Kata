# Read the keywords.txt file and generate keywords.c and keywords.h
import sys

raw_list = []
with open(sys.argv[1], "r") as inf:
    for line in inf:
        s = line.strip()
        if len(s) > 0:
            raw_list.append(s)

raw_list.sort()
#print (raw_list)

with open("tokens.h", "w") as outf:

    outf.write('''
/* This file is generated with a script. Do not edit. */
#ifndef _TOKENS_H
#define _TOKENS_H
#include "util.h"

typedef enum {
    // markers
    TOK_END_OF_FILE,
    TOK_END_OF_INPUT,
    TOK_ERROR,\n
    // keywords
''')

    for item in raw_list:
        outf.write("    TOK_%s,\n"%(item.upper()))

    outf.write('''

    // operators
    TOK_LORE,
    TOK_GORE,
    TOK_EQU,
    TOK_NEQU,
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
    TOK_LITERAL_SIGNED,
    TOK_LITERAL_UNSIGNED,
    TOK_LITERAL_DSTR,
    TOK_LITERAL_SSTR,
    TOK_SYMBOL,
    TOK_UNARY_MINUS,
} TokenType;

/**
 * @brief The parser expects a token to look like this.
 */
typedef struct {
    const char* str;
    TokenType type;
} KeywordList;

/**
 * @brief The parser expects a token to look like this.
 */
typedef struct {
    Str* str;          // String that caused the token to be recognized
    TokenType type;    // Type of the token
    int line_no;       // Line number where the token was recognized
    int col_no;        // Column of the last character of the token
    const char* fname; // File name where the token was taken
} Token;

extern KeywordList keyword_list[];
extern const int num_keywords;

const char* tok_to_str(Token* tok);

''')

    outf.write("#endif /* _TOKENS_H */\n\n")

with open("tokens.c", "w") as outf:
    outf.write("/* This file is generated with a script. Do not edit. */\n\n")
    outf.write("#include \"tokens.h\"\n\n")
    outf.write("KeywordList keyword_list[] = {\n")
    for item in raw_list:
        outf.write("    { \"%s\", TOK_%s },\n"%(item, item.upper()))
    outf.write("};\n\n")
    outf.write("const int num_keywords = (sizeof(keyword_list) / sizeof(KeywordList));\n\n")

    outf.write("const char* tok_to_str(Token* tok) {\n")
    outf.write("    assert(tok != NULL);\n\n")
    outf.write("    return\n        /* keywords */\n")
    for item in raw_list:
        outf.write("        (tok->type == TOK_%s)? \"%s\":\n"%(item.upper(), item))

    outf.write('''
        /* other tokens */
        (tok->type == TOK_END_OF_FILE)? "end of file":
        (tok->type == TOK_END_OF_INPUT)? "end of input":
        (tok->type == TOK_ERROR)? "error":
        (tok->type == TOK_LORE)? "<=":
        (tok->type == TOK_GORE)? ">=":
        (tok->type == TOK_EQU)? "==":
        (tok->type == TOK_NEQU)? "!=":
        (tok->type == TOK_OR)? "or":
        (tok->type == TOK_AND)? "and":
        (tok->type == TOK_NOT)? "not":
        (tok->type == TOK_ADD_ASSIGN)? "+=":
        (tok->type == TOK_SUB_ASSIGN)? "-=":
        (tok->type == TOK_MUL_ASSIGN)? "*=":
        (tok->type == TOK_DIV_ASSIGN)? "/=":
        (tok->type == TOK_MOD_ASSIGN)? "%=":
        (tok->type == TOK_ADD)? "+":
        (tok->type == TOK_SUB)? "-":
        (tok->type == TOK_ASSIGN)? "=":
        (tok->type == TOK_DIV)? "/":
        (tok->type == TOK_MUL)? "*":
        (tok->type == TOK_MOD)? "%":
        (tok->type == TOK_OPAREN)? "(":
        (tok->type == TOK_CPAREN)? ")":
        (tok->type == TOK_OCBRACE)? "{":
        (tok->type == TOK_CCBRACE)? "}":
        (tok->type == TOK_OSBRACE)? "[":
        (tok->type == TOK_CSBRACE)? "]":
        (tok->type == TOK_COMMA)? ",":
        (tok->type == TOK_DOT)? ".":
        (tok->type == TOK_OPBRACE)? "<":
        (tok->type == TOK_CPBRACE)? ">":
        (tok->type == TOK_COLON)? ":":
        (tok->type == TOK_CARAT)? "^":
        (tok->type == TOK_AMPER)? "&":
        (tok->type == TOK_LITERAL_FLOAT)? "literal float":
        (tok->type == TOK_LITERAL_SIGNED)? "literal signed":
        (tok->type == TOK_LITERAL_UNSIGNED)? "literal unsigned":
        (tok->type == TOK_LITERAL_DSTR)? "literal dstrg":
        (tok->type == TOK_LITERAL_SSTR)? "literal sstrg":
        (tok->type == TOK_SYMBOL)? "symbol":
        (tok->type == TOK_UNARY_MINUS)? "unary minus": "UNKNOWN";\n}\n\n
''')

