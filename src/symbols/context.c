/**
 * @file context.c
 * 
 * @brief Manage the context of symbols. The symbolic context is stored in a 
 * stack. When a symbol is seen it is pushed on the stack with the AST node 
 * that contains the pertinent information about the symbol. When the symbol 
 * goes out of scope then it is popped off of the stack. This context stack 
 * is implemented as an opaque data structure and all actions that can be 
 * performed on it are provided as methods.
 * 
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-05-18
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdlib.h>

#include "memory.h"
#include "context.h"

static SymContext* _local_context = NULL;

SymContext* create_sym_context() {

}

void push_sym_context(const char* name, ast_node* node) {

}

SymContextNode* pop_sym_context() {

}

SymContextNode* peek_sym_context() {

}

SymContext* copy_sym_context(SymContext* ptr) {

}

const char* get_sym_context(SymContext* ptr) {
    
}
