/**
 * @file keyword.c
 *
 * @brief Data storeage for the keywords.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-11-2024
 * @copyright Copyright (c) 2024
 */
#include "keyword.h"

/**
 * @brief The keyword list is sorted by the parser generator. (me in this case)
 */
KeywordList keyword_list[] = {
    { "and", TOK_AND },
    { "as", TOK_AS },
    { "bool", TOK_BOOLEAN },
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
    { "false", TOK_FALSE_BOOL },
    { "for", TOK_FOR },
    { "if", TOK_IF },
    { "import", TOK_IMPORT },
    { "in", TOK_IN },
    { "inline", TOK_INLINE },
    { "list", TOK_LIST },
    { "namespace", TOK_NAMESPACE },
    { "noth", TOK_NOTHING },
    { "nothing", TOK_NOTHING },
    { "num", TOK_NUMBER },
    { "number", TOK_NUMBER },
    { "off", TOK_FALSE_BOOL },
    { "on", TOK_TRUE_BOOL },
    { "or", TOK_OR },
    { "print", TOK_PRINT },
    { "private", TOK_PRIVATE },
    { "protected", TOK_PROTECTED },
    { "public", TOK_PUBLIC },
    { "raise", TOK_RAISE },
    { "return", TOK_RETURN },
    { "start", TOK_START },
    { "strg", TOK_STRING },
    { "string", TOK_STRING },
    { "struct", TOK_STRUCT },
    { "switch", TOK_SWITCH },
    { "to", TOK_TO },
    { "trace", TOK_TRACE },
    { "true", TOK_TRUE_BOOL },
    { "try", TOK_TRY },
    { "type", TOK_TYPE },
    { "virtual", TOK_VIRTUAL },
    { "while", TOK_WHILE },
    { "yield", TOK_YIELD },
};

const int num_keywords = (sizeof(keyword_list) / sizeof(KeywordList));
