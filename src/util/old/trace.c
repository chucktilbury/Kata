
#include "util.h"

int trace_count           = 0;
const int trace_increment = 4;
LList trace_state_stack = NULL;

void push_trace_state(bool state) {

    if(NULL == trace_state_stack)
        trace_state_stack = create_llist();

    bool* node = _ALLOC_T(bool);
    *node = state;

    push_llist(trace_state_stack, node);
}

bool pop_trace_state() {

    bool* state = pop_llist(trace_state_stack);

    if(state != NULL)
        return *state;
    else
        return false;
}

bool peek_trace_state() {

    bool* state = peek_llist(trace_state_stack);

    if(state != NULL)
        return *state;
    else
        return false;
}

// silently fail if there are no states on the stack.
void set_trace_state(bool state) {

    bool* node = peek_llist(trace_state_stack);

    if(node != NULL)
        *node = state;
}