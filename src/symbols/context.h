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

#include "ast.h"

typedef struct {
    ast_node* node;
    const char* name;
} SymContextNode;

typedef struct {
    SymContextNode* list;
    int cap;
    int len;
} SymContext;

SymContext* create_sym_context();
void push_sym_context(const char* name, ast_node* node);
SymContextNode* pop_sym_context();
SymContextNode* peek_sym_context();
SymContext* copy_sym_context(SymContext* ptr);
const char* get_sym_context(SymContext* ptr);


#endif  /* _CONTEXT_H_ */
