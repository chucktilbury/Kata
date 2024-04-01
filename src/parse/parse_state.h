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

#include "sstrings.h"
#include "link_list.h"

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
    LinkList* scope_stack;
    LinkList* name_stack;
    bool is_import;
} ParserState;

ParserState* create_parser_state();
void set_scope(ScopeType scope);
void push_scope(ScopeType scope);
ScopeType pop_scope();
ScopeType get_scope();
void push_name(String* name);
String* pop_name();
String* get_name();
String* get_compound_name();
void set_import_state(bool val);
const char* scope_name(ScopeType type);

#endif /* __PARSE_STATE_H__ */
