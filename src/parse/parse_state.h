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
 * funcitons.
 */
typedef struct {
    ScopeType scope;
    bool is_import;
} ParserState;

void set_scope(ScopeType scope);
ScopeType get_scope();

#endif /* __PARSE_STATE_H__ */
