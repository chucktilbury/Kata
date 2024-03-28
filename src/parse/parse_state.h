/**
 * @file parse_state.h
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-29-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __PARSE_STATE_H__
#define __PARSE_STATE_H__

/**
 * @brief The scope is a state that is tracked as items are parsed.
 */
typedef enum {
    SCOPE_PUB,
    SCOPE_PRIV,
    SCOPE_PROT,
} ScopeType;

/**
 * @brief This data structure is a stub to template passing a state to parse
 * functions.
 */
typedef struct {
    LList scope_stack;
    LList name_stack;
    bool is_import;
} ParserState;

ParserState* create_parser_state();
void set_scope(ScopeType scope);
void push_scope(ScopeType scope);
ScopeType pop_scope();
ScopeType get_scope();
void push_name(Str* name);
Str* pop_name();
Str* get_name();
Str* get_compound_name();

#endif /* __PARSE_STATE_H__ */
