/**
 * @file context.c
 * 
 * @brief Manage the context of symbols. The symbolic context is stored in a 
 * stack. When a symbol is seen it is pushed on the stack with the AST node 
 * that contains the pertinent information about the symbol. When the symbol 
 * goes out of scope then it is popped off of the stack. This context stack 
 * is implemented as an opaque data structure and all actions that can be 
 * performed on it are provided as methods. Every time a name is declared or 
 * referenced, it must have a valid context. The whole symbol context is 
 * stored in the symbol table to make each variable unique. When the code is
 * translated to C, every variable uses the context to identify it uniquely.
 * 
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-05-18
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "context.h"

static SymContext* _local_context = NULL;

typedef struct {
    char* buffer;
    int cap;
    int len;
} _str_buffer;

/**
 * @brief Append a string to the buffer.
 * 
 * @param ptr 
 * @param str 
 */
static inline void _add_str_buffer(_str_buffer* ptr, const char* str) {

    size_t len = strlen(str);

    if((ptr->len + len + 1) >= (size_t)ptr->cap) {
        while((ptr->len + len + 1) >= (size_t)ptr->cap)
            ptr->cap <<= 1;
        ptr->buffer = _REALLOC_ARRAY(ptr->buffer, char, ptr->cap);
    }

    memcpy(&ptr->buffer[ptr->len], str, len + 1);
    ptr->len += len;
}

/**
 * @brief Add a character to the string buffer.
 * 
 * @param ptr 
 * @param ch 
 */
static inline void _add_char_buffer(_str_buffer* ptr, int ch) {

    if((ptr->len + 1) >= ptr->cap) {
        ptr->cap <<= 1;
        ptr->buffer = _REALLOC_ARRAY(ptr->buffer, char, ptr->cap);
    }

    ptr->buffer[ptr->len] = (char)ch;
    ptr->len ++;
}

/**
 * @brief Create a str buffer object
 * 
 * @return _str_buffer* 
 */
static inline _str_buffer* _create_str_buffer(const char* str) {

    _str_buffer* ptr = _ALLOC_T(_str_buffer);
    ptr->cap = 0x01 << 3;
    ptr->len = 0;
    ptr->buffer = _ALLOC_ARRAY(char, ptr->cap);

    if(str != NULL)
        _add_str_buffer(ptr, str);

    return ptr;
}

/**
 * @brief Append the string to the string list and grow the list if required.
 * 
 * @param ptr 
 * @param str 
 */
static inline void _append_sym_context(SymContext* ptr, const char* str) {

    if(ptr->len+1 >= ptr->cap) {
        ptr->cap <<= 1;
        ptr->list = _REALLOC_ARRAY(ptr->list, const char*, ptr->cap);
    }

    ptr->list[ptr->len] = _DUP_STR(str);
    ptr->len++;
}

/******************************************************************************
 * Public Interface
 */

/**
 * @brief Allocate memory for a new sym context object and push the name on
 * the stack. If the name is NULL, then only add the AST node.
 * 
 * @return SymContext* 
 */
SymContext* create_sym_context(const char* name) {

    SymContext* ptr = _ALLOC_T(SymContext);
    ptr->cap = 0x01 << 3;
    ptr->len = 0;
    ptr->list = _ALLOC_ARRAY(const char*, ptr->cap);

    if(name != NULL) 
        _append_sym_context(ptr, name);

    return ptr;
}

/**
 * @brief Create a new context and push it on the stack. This is used to 
 * maintain the current context as the AST is being traversed. 
 * 
 * @param name 
 * @param node 
 */
void push_sym_context(const char* name) {

    _append_sym_context(_local_context, name);
}

/**
 * @brief Return the top of stack element and remove it from the stack. This 
 * is called when the current context goes out of scope.
 * 
 * @return SymContextNode* 
 */
const char* pop_sym_context() {

    if(_local_context->len > 0) {
        _local_context->len--;
        return _local_context->list[_local_context->len];
    }
    else
        return NULL;
}

/**
 * @brief Return the top of stack element but do not remove it.
 * 
 * @return SymContextNode* 
 */
const char* peek_sym_context() {

    if(_local_context->len > 0) 
        return _local_context->list[_local_context->len-1];
    else
        return NULL;
}

/**
 * @brief Create a copy of the given context and return it. After a context 
 * has been coppied, it can be pushed and popped like any other.
 * 
 * @param ptr 
 * @return SymContext* 
 */
SymContext* copy_sym_context(SymContext* ptr) {

    SymContext* ctx = create_sym_context(NULL);

    int i = 0;
    const char* name = NULL;
    while(NULL != (name = iterate_sym_context(ptr, &i)))
        _append_sym_context(ctx, name);

    return ctx;
}

/**
 * @brief Return the context as a normal C string. For example to store it.
 * Returns a memory-allocated buffer. Cound be zero length with no error.
 * 
 * @param ptr 
 * @return const char* 
 */
const char* get_sym_context(SymContext* ptr) {
    
    _str_buffer* buf = _create_str_buffer(NULL);
    int i = 0;
    const char* name = iterate_sym_context(ptr, &i);

    // build the return value
    while(1) {
        _add_str_buffer(buf, name);
        if(NULL != (name = iterate_sym_context(ptr, &i)))
            _add_char_buffer(buf, '.');
        else
            break;
    }

    return buf->buffer;
}

/**
 * @brief Add a string to the current context. Even if the string has '.' in 
 * it, this function will properly add the segments as part of the context.
 * 
 * @param ptr 
 * @param str 
 */
void add_sym_context(SymContext* ptr, const char* str) {

    _str_buffer* buf = _create_str_buffer(str);
    char* buffer = buf->buffer;
    char* tok;

    while(NULL != (tok = strtok(buffer, "."))) {
        _append_sym_context(ptr, tok);
        buffer = NULL;
    }
}

/**
 * @brief Iterate the context. The post needs to be an int and 0 on the first 
 * iteration. Function is reentrant.
 * 
 * @param ptr 
 * @param post 
 * @return const char* 
 */
const char* iterate_sym_context(SymContext* ptr, int* post) {

    if(ptr->len > *post) 
        return(ptr->list[*(post)++]);
    else
        return NULL;
}

