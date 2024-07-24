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
#ifndef _PARSE_STATE_H_
#define _PARSE_STATE_H_

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
    bool in_recovery;
} ParserState;

ParserState* create_parser_state(void);

void set_scope(ScopeType scope);
void push_scope(ScopeType scope);
ScopeType pop_scope(void);
ScopeType get_scope(void);
const char* scope_name(ScopeType type);

void push_name(String* name);
String* pop_name(void);
String* get_name(void);
String* get_compound_name(void);

void set_import_state(bool val);
bool get_import_state(void);

void set_recovery_state(bool state);
bool get_recovery_state(void);

#endif  /* _PARSE_STATE_H_ */
