/**
 * @file trace.c
 *
 * @brief Print statements for debugging.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-29-2024
 * @copyright Copyright (c) 2024
 */
#include <assert.h>

#include "link_list.h"
#include "memory.h"

#include "ast.h"
#include "scanner.h"
#include "trace.h"

int trace_count             = 0;
const int trace_increment   = 4;
LinkList* trace_state_stack = NULL;

void push_trace_state(bool state) {

    if(NULL == trace_state_stack)
        trace_state_stack = create_link_list();

    bool* node = _ALLOC_T(bool);
    *node      = state;

    push_link_list(trace_state_stack, node);
}

bool pop_trace_state(void) {

    bool* state = pop_link_list(trace_state_stack);

    if(state != NULL)
        return *state;
    else
        return false;
}

bool peek_trace_state(void) {

    bool* state = peek_link_list(trace_state_stack);

    if(state != NULL)
        return *state;
    else
        return false;
}

// silently fail if there are no states on the stack.
void set_trace_state(bool state) {

    bool* node = peek_link_list(trace_state_stack);

    if(node != NULL)
        *node = state;
}

/**
 * @brief Print the content of the token for debugging.
 *
 * @param tok
 */
void print_terminal(void* ptr) {

    assert(ptr != NULL);
    Token* tok = (Token*)ptr;

    printf("%s: \"%s\": %d: %d: \"%s\"", tok_to_str(tok), raw_string(tok->str),
           tok->line_no, tok->col_no, tok->fname);
}

void print_nonterminal(void* ptr) {

    assert(ptr != NULL);
}
