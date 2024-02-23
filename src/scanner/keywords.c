/* This file is generated with a script. Do not edit. */

#include "tokens.h"

KeywordList keyword_list[] = {
    { "and", TOK_AND },
    { "as", TOK_AS },
    { "bool", TOK_BOOL },
    { "boolean", TOK_BOOLEAN },
    { "break", TOK_BREAK },
    { "case", TOK_CASE },
    { "class", TOK_CLASS },
    { "const", TOK_CONST },
    { "continue", TOK_CONTINUE },
    { "create", TOK_CREATE },
    { "default", TOK_DEFAULT },
    { "destroy", TOK_DESTROY },
    { "dict", TOK_DICT },
    { "do", TOK_DO },
    { "else", TOK_ELSE },
    { "except", TOK_EXCEPT },
    { "exit", TOK_EXIT },
    { "false", TOK_FALSE },
    { "float", TOK_FLOAT },
    { "for", TOK_FOR },
    { "if", TOK_IF },
    { "import", TOK_IMPORT },
    { "in", TOK_IN },
    { "inline", TOK_INLINE },
    { "list", TOK_LIST },
    { "namespace", TOK_NAMESPACE },
    { "not", TOK_NOT },
    { "noth", TOK_NOTH },
    { "nothing", TOK_NOTHING },
    { "off", TOK_OFF },
    { "on", TOK_ON },
    { "or", TOK_OR },
    { "print", TOK_PRINT },
    { "private", TOK_PRIVATE },
    { "protected", TOK_PROTECTED },
    { "public", TOK_PUBLIC },
    { "raise", TOK_RAISE },
    { "return", TOK_RETURN },
    { "signed", TOK_SIGNED },
    { "start", TOK_START },
    { "strg", TOK_STRG },
    { "string", TOK_STRING },
    { "struct", TOK_STRUCT },
    { "switch", TOK_SWITCH },
    { "to", TOK_TO },
    { "trace", TOK_TRACE },
    { "true", TOK_TRUE },
    { "try", TOK_TRY },
    { "type", TOK_TYPE },
    { "unsigned", TOK_UNSIGNED },
    { "virtual", TOK_VIRTUAL },
    { "while", TOK_WHILE },
    { "yield", TOK_YIELD },
};

const int num_keywords = (sizeof(keyword_list) / sizeof(KeywordList));

const char* tok_to_str(Token* tok) {
    assert(tok != NULL);

    return
        /* keywords */
        (tok->type == TOK_AND)? "and":
        (tok->type == TOK_AS)? "as":
        (tok->type == TOK_BOOL)? "bool":
        (tok->type == TOK_BOOLEAN)? "boolean":
        (tok->type == TOK_BREAK)? "break":
        (tok->type == TOK_CASE)? "case":
        (tok->type == TOK_CLASS)? "class":
        (tok->type == TOK_CONST)? "const":
        (tok->type == TOK_CONTINUE)? "continue":
        (tok->type == TOK_CREATE)? "create":
        (tok->type == TOK_DEFAULT)? "default":
        (tok->type == TOK_DESTROY)? "destroy":
        (tok->type == TOK_DICT)? "dict":
        (tok->type == TOK_DO)? "do":
        (tok->type == TOK_ELSE)? "else":
        (tok->type == TOK_EXCEPT)? "except":
        (tok->type == TOK_EXIT)? "exit":
        (tok->type == TOK_FALSE)? "false":
        (tok->type == TOK_FLOAT)? "float":
        (tok->type == TOK_FOR)? "for":
        (tok->type == TOK_IF)? "if":
        (tok->type == TOK_IMPORT)? "import":
        (tok->type == TOK_IN)? "in":
        (tok->type == TOK_INLINE)? "inline":
        (tok->type == TOK_LIST)? "list":
        (tok->type == TOK_NAMESPACE)? "namespace":
        (tok->type == TOK_NOT)? "not":
        (tok->type == TOK_NOTH)? "noth":
        (tok->type == TOK_NOTHING)? "nothing":
        (tok->type == TOK_OFF)? "off":
        (tok->type == TOK_ON)? "on":
        (tok->type == TOK_OR)? "or":
        (tok->type == TOK_PRINT)? "print":
        (tok->type == TOK_PRIVATE)? "private":
        (tok->type == TOK_PROTECTED)? "protected":
        (tok->type == TOK_PUBLIC)? "public":
        (tok->type == TOK_RAISE)? "raise":
        (tok->type == TOK_RETURN)? "return":
        (tok->type == TOK_SIGNED)? "signed":
        (tok->type == TOK_START)? "start":
        (tok->type == TOK_STRG)? "strg":
        (tok->type == TOK_STRING)? "string":
        (tok->type == TOK_STRUCT)? "struct":
        (tok->type == TOK_SWITCH)? "switch":
        (tok->type == TOK_TO)? "to":
        (tok->type == TOK_TRACE)? "trace":
        (tok->type == TOK_TRUE)? "true":
        (tok->type == TOK_TRY)? "try":
        (tok->type == TOK_TYPE)? "type":
        (tok->type == TOK_UNSIGNED)? "unsigned":
        (tok->type == TOK_VIRTUAL)? "virtual":
        (tok->type == TOK_WHILE)? "while":
        (tok->type == TOK_YIELD)? "yield":

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
        (tok->type == TOK_LITERAL_DSTR)? "literal dstrg":
        (tok->type == TOK_LITERAL_SSTR)? "literal sstrg":
        (tok->type == TOK_SYMBOL)? "symbol":
        (tok->type == TOK_UNARY_MINUS)? "unary minus": "UNKNOWN";
}


