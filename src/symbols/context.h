/**
 * @file context.h
 *
 * @brief Public interface for managing symbolic context of symbols.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-05-18
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _CONTEXT_H_
#define _CONTEXT_H_

typedef struct {
    const char** list;
    int cap;
    int len;
} SymContext;

#include "ast.h"

void init_global_context();
SymContext* create_sym_context(const char* name);
void push_sym_context(const char* name);
const char* pop_sym_context();
const char* peek_sym_context();
SymContext* root_sym_context();
SymContext* copy_sym_context(SymContext* ptr);
SymContext* copy_root_context();
const char* get_sym_context(SymContext* ptr);
void add_sym_context(SymContext* ptr, const char* str);
const char* iterate_sym_context(SymContext* ptr, int* post);

#endif /* _CONTEXT_H_ */
