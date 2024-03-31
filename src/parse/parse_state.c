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

    ENTER;
    ScopeType* node = peek_link_list(parser_state->scope_stack);
    assert(node != NULL);   // should never happen

    *node = scope;
    RET;
}

/**
 * @brief Push the scope when a scope block is entered.
 *
 * @param scope
 *
 */
void push_scope(ScopeType scope) {

    ENTER;
    ScopeType* node = _ALLOC_T(ScopeType);
    *node = scope;

    TRACE("scope: %d", scope);
    push_link_list(parser_state->scope_stack, node);
    RET;
}

/**
 * @brief Pop the scope when a scope block is completed.
 *
 * @return ScopeType
 *
 */
ScopeType pop_scope() {

    ENTER;
    ScopeType* node = pop_link_list(parser_state->scope_stack);
    assert(node != NULL);

    RETV(*node);
}

/**
 * @brief Get the scope object
 *
 * @return ScopeType
 *
 */
ScopeType get_scope() {

    ENTER;
    ScopeType* node = peek_link_list(parser_state->scope_stack);
    assert(node != NULL);

    RETV(*node);
}

/**
 * @brief Create a parser state object
 *
 * @return ParserState*
 *
 */
ParserState* create_parser_state() {

    ENTER;
    parser_state = _ALLOC_T(ParserState);

    parser_state->scope_stack = create_link_list();
    push_scope(SCOPE_PRIV);

    parser_state->name_stack = create_link_list();
    push_name(create_string("root"));

    parser_state->is_import = false;

    RETV(parser_state);
}

void push_name(String* name) {

    ENTER;
    TRACE("name: %s", name->buffer);
    push_link_list(parser_state->name_stack, name);
    RET;
}

String* pop_name() {

    ENTER;
    RETV(pop_link_list(parser_state->name_stack));
}

String* get_name() {

    ENTER;
    RETV(peek_link_list(parser_state->name_stack));
}

String* get_compound_name() {

    ENTER;
    String* str = create_string(NULL);
    String* sptr;
    StrList* lptr = create_string_list();
    void* mark = NULL;

    while(NULL != (sptr = iter_link_list(parser_state->name_stack, &mark)))
        push_string_list(lptr, sptr);

    mark = NULL;
    while(NULL != (sptr = iter_link_list(lptr, &mark))) {
        TRACE("segment: %s", sptr->buffer);
        add_string_Str(str, sptr);
        add_string_char(str, '.');
    }

    TRACE("compound name: \"%s\"", str->buffer);
    RETV(str);
}

void set_import_state(bool val) {

    ENTER;
    parser_state->is_import = val;
    RET;
}

