/**
 * @file except.h
 *
 * @brief This implementation of exceptions is used to handle errors. There
 * are no classes in C. This uses a global enum to register an exception
 * handler by name. Using the non-local goto functionality and the
 * preprocessor of C, a reasonably easy to maintain exception capability can
 * be achieved. This has to use macros because the current context in the
 * function that handles the exceptions is required.
 *
 * These data structures should never be accessed directly. They need to be
 * defined here because of the macros.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-29-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <setjmp.h>
#include <string.h>
#include "memory.h"

typedef struct _exception_stack_ {
    jmp_buf jmp;
    struct _exception_stack_* next;
} _ExceptionStack;

typedef struct {
    _ExceptionStack* stack;
    const char* msg;
    const char* file;
    const char* func;
    int line;
} _ExceptionState;

// defined in exceptions.c
// This prevents exceptions from being thread safe without a lot more work.
extern _ExceptionState _exception_state;

// Set up a try block
#define TRY                                                            \
    do {                                                               \
        _ExceptionStack* _exception_ptr = _ALLOC_T(_ExceptionStack);   \
        _exception_ptr->next            = _exception_state.stack;      \
        _exception_state.stack          = _exception_ptr;              \
        int _exception_number           = setjmp(_exception_ptr->jmp); \
        if(_exception_number == 0)

// Catch a specific exception
#define EXCEPT(num) else if(_exception_number == (num))

// Catch any exception. This can be used with the FINAL macro or it can be
// ued instead of it.
#define ANY_EXCEPT() else if(_exception_number != 0)

// FINAL and/or ANY_EXCEPT clause is REQUIRED for the system to work, and it
// MUST be the last clause in the construct.
#define FINAL                                                                               \
    else {                                                                                  \
        if(_exception_state.stack == NULL) {                                                \
            fprintf(stderr, "Unhandled Exception: 0x%04X: %s: %s: %d: %s\n", EXCEPTION_NUM, \
                    EXCEPTION_FILE, EXCEPTION_FUNC, EXCEPTION_LINE, EXCEPTION_MSG);         \
            abort();                                                                        \
        }                                                                                   \
        else {                                                                              \
            INTERNAL_RAISE(_exception_number);                                              \
        }                                                                                   \
    }                                                                                       \
    }                                                                                       \
    while(0)                                                                                \
        ;

// use this to raise an exception
#define RAISE(num, m, ...)                                  \
    do {                                                    \
        _exception_state.line = __LINE__;                   \
        if(_exception_state.file != NULL)                   \
            _FREE(_exception_state.file);                   \
        _exception_state.file = _DUP_STR(__FILE__);         \
        if(_exception_state.func != NULL)                   \
            _FREE(_exception_state.func);                   \
        _exception_state.func = _DUP_STR(__func__);         \
        if(_exception_state.msg != NULL)                    \
            _FREE(_exception_state.msg);                    \
        _exception_state.msg = _FDUP_STR(m, ##__VA_ARGS__); \
        INTERNAL_RAISE(num);                                \
    } while(0)

// internal use only
#define INTERNAL_RAISE(num)                                                         \
    do {                                                                            \
        jmp_buf buf;                                                                \
        _ExceptionStack* ptr = _exception_state.stack;                              \
        if(ptr != NULL)                                                             \
            memcpy(buf, ptr->jmp, sizeof(jmp_buf));                                 \
        else {                                                                      \
            fprintf(stderr, "Unhandled Exception: 0x%04X: %s: %s: %d: %s\n", num,   \
                    EXCEPTION_FILE, EXCEPTION_FUNC, EXCEPTION_LINE, EXCEPTION_MSG); \
            abort();                                                                \
        }                                                                           \
        _exception_state.stack = ptr->next;                                         \
        _FREE(ptr);                                                                 \
        longjmp(buf, (num));                                                        \
    } while(0)

// use these macros in your exception handler
#define EXCEPTION_MSG _exception_state.msg
#define EXCEPTION_FILE _exception_state.file
#define EXCEPTION_LINE _exception_state.line
#define EXCEPTION_FUNC _exception_state.func
#define EXCEPTION_NUM _exception_number

typedef enum {
    GENERIC_ERROR,
    MEMORY_ERROR,
    FILE_ERROR,
    LIST_ERROR,
} UTIL_EXCEPTIONS;


#endif /* __EXCEPTION_H__ */
