/**
 * @file parse_state.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-29-2024
 * @copyright Copyright (c) 2024a
 */
#include <assert.h>

#include "trace.h"
#include "memory.h"
#include "parse_state.h"

ParserState* parser_state = NULL;


/**
 * @brief Set the scope object
 *
 * @param scope
 *
 */
void set_scope(ScopeType scope) {

    ScopeType* node = peek_link_list(parser_state->scope_stack);
    assert(node != NULL);   // should never happen

    *node = scope;
}

/**
 * @brief Push the scope when a scope block is entered.
 *
 * @param scope
 *
 */
void push_scope(ScopeType scope) {

    ScopeType* node = _ALLOC_T(ScopeType);
    *node = scope;

    push_link_list(parser_state->scope_stack, node);
}

/**
 * @brief Pop the scope when a scope block is completed.
 *
 * @return ScopeType
 *
 */
ScopeType pop_scope() {

    ScopeType* node = pop_link_list(parser_state->scope_stack);
    assert(node != NULL);

    return *node;
}

/**
 * @brief Get the scope object
 *
 * @return ScopeType
 *
 */
ScopeType get_scope() {

    ScopeType* node = peek_link_list(parser_state->scope_stack);
    assert(node != NULL);

    return *node;
}

/**
 * @brief Create a parser state object
 *
 * @return ParserState*
 *
 */
ParserState* create_parser_state() {

    parser_state = _ALLOC_T(ParserState);

    parser_state->scope_stack = create_link_list();
    push_scope(SCOPE_PRIV);

    parser_state->name_stack = create_link_list();
    push_name(create_string("root"));

    parser_state->is_import = false;

    return parser_state;
}

void push_name(String* name) {

    push_link_list(parser_state->name_stack, name);
}

String* pop_name() {

    return pop_link_list(parser_state->name_stack);
}

String* get_name() {

    return peek_link_list(parser_state->name_stack);
}

String* get_compound_name() {

    String* str = create_string(NULL);
    String* sptr;
    StrList* lptr = create_string_list();
    void* mark = NULL;

    while(NULL != (sptr = iter_link_list(parser_state->name_stack, &mark)))
        push_string_list(lptr, sptr);

    mark = NULL;
    while(NULL != (sptr = iter_link_list(lptr, &mark))) {
        add_string_Str(str, sptr);
        add_string_char(str, '.');
    }

    return str;
}

