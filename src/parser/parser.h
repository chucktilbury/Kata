/**
 * @file parser.h
 *
 * @brief This is the header file for the parser.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-13-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _PARSER_H
#define _PARSER_H

typedef enum {
    NTT_SCOPE_OPERATOR,
    NTT_TYPE_NAME,
    NTT_TYPE_SPEC_ELEMENT,
    NTT_TYPE_SPEC,
    NTT_COMPOUND_NAME,
} NonTermType;

typedef struct {
    NonTermType type;
    void* data;
} NonTerm;

typedef struct {
    Token* tok;
} ScopeOperator;

typedef struct {
    Token* tok;
} TypeName;

typedef struct {
    StrList* lst;
} CompoundName;

#endif /* _PARSER_H */
