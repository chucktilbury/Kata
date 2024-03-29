#ifndef _UTIL_H
#define _UTIL_H

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//----------------------------------------------
// mem.c
//----------------------------------------------
#if 1
#define _ALLOC(s) mem_alloc(s)
#define _ALLOC_T(t) (t*)mem_alloc(sizeof(t))
#define _ALLOC_ARRAY(t, n) (t*)mem_alloc(sizeof(t) * (n))
#define _REALLOC(p, s) mem_realloc((p), (s))
#define _REALLOC_T(p, t) (t*)mem_realloc((p), sizeof(t))
#define _REALLOC_ARRAY(p, t, n) (t*)mem_realloc((p), sizeof(t) * (n))
#define _DUP_MEM(p, s) mem_dup((p), (s))
#define _DUP_MEM_T(p, t) (t*)mem_dup((p), sizeof(t))
#define _DUP_MEM_ARRAY(p, t, n) (t*)mem_dup((p), sizeof(t) * (n))
#define _DUP_STR(p) mem_dup_str(p)
#define _FDUP_STR(p, ...) mem_fdup_str(p __VA_OPT__(,) __VA_ARGS__)
#define _FREE(p) mem_free(((void*)p))

void* mem_alloc(size_t size);
void* mem_realloc(void* ptr, size_t size);
void* mem_dup(void* ptr, size_t size);
char* mem_dup_str(const char* str);
char* mem_fdup_str(const char* str, ...);
void mem_free(void* ptr);
#endif

//------------------------------------------------------
// datalst.c
//------------------------------------------------------
// Arbitrary sized data buffer list. All of the elements
// must be the same size. Copies of the data is saved in
// the list.
typedef struct {
    unsigned char* buffer; // buffer that holds the raw bytes.
    int cap;               // number of bytes there is room for
    int len;               // number of bytes in the list.
    int size;              // number of bytes that each item uses.
    bool changed;          // used when iterating data
} List;

typedef struct {
    List* list; // list to iterate
    int index;  // current index of the data in the list
} ListIter;

List* create_list(int size);
void destroy_list(List* lst);
void append_list(List* lst, void* data);
void read_list(List* lst, int index, void* data);
void write_list(List* lst, int index, void* data);
void insert_list(List* lst, int index, void* data);
void delete_list(List* lst, int index);
void push_list(List* lst, void* data);
void peek_list(List* lst, void* data);
void pop_list(List* lst, void* data);
void clear_list(List* lst);
// TODO: compare, swap, bubble sort, and binary search. This will
// require that a user-defined compare function be stored in the
// data structure.

// iterator
ListIter* init_list_iterator(List* lst);
int iterate_list(ListIter* iter, void* data);
ListIter* init_list_riterator(List* lst);
int riterate_list(ListIter* iter, void* data);
// info about the list
void* raw_list(List* lst);
int length_list(List* lst);

//------------------------------------------------------
// ptrlst.c
//------------------------------------------------------
// generic pointer list.
typedef List PtrList;
typedef ListIter PtrListIter;

static inline PtrList* create_ptr_list() {
    return create_list(sizeof(void*));
}

static inline void destroy_ptr_list(PtrList* h) {
    destroy_list(h);
}

static inline void add_ptr_list(PtrList* h, void* ptr) {
    append_list(h, &ptr);
}

static inline PtrListIter* init_ptr_list_iterator(PtrList* h) {
    return init_list_iterator(h);
}

static inline void* iterate_ptr_list(PtrListIter* ptr) {
    void* val;
    if(iterate_list(ptr, &val))
        return val;
    else
        return NULL;
}

static inline void push_ptr_list(PtrList* h, void* ptr) {
    push_list(h, &ptr);
}

static inline void* peek_ptr_list(PtrList* h) {
    void* val = NULL;
    peek_list(h, &val);
    return val; // note that this is not a local pointer value.
}

static inline void* pop_ptr_list(PtrList* h) {
    void* val = NULL;
    pop_list(h, &val);
    return val; // note that this is not a local pointer value.
}

//--------------------------------------------------------
// llist.c
//--------------------------------------------------------
#if 1
// Specialize the ptr list to be a str list.
typedef void* LList;
LList create_llist();
void append_llist(LList ll, void* data);
void prepend_llist(LList ll, void* data);
void push_llist(LList ll, void* data);
void* pop_llist(LList ll);
void* peek_llist(LList ll);
void* init_llist_iter(LList ll);
void* iter_llist(LList ll);
int len_llist(LList ll);
#endif

//--------------------------------------------------------
// str.c
//--------------------------------------------------------
// Specialize the ptr list to be a str list.
typedef List StrList;
typedef ListIter StrListIter;
typedef List Str;
typedef ListIter StrIter;

Str* join_string_list(StrList* lst, const char* str);
Str* copy_string(Str* str);
Str* create_string(const char* str);
Str* create_string_fmt(const char* str, ...);
void destroy_string(Str* ptr);
void add_string_char(Str* ptr, int ch);
void add_string_str(Str* ptr, const char* str);
void add_string_fmt(Str* ptr, const char* str, ...);

StrIter* init_string_iterator(Str* ptr);
int iterate_string(StrIter* ptr);

const char* raw_string(Str* ptr);
int comp_string(Str* s1, Str* s2);
int comp_string_const(Str* s1, const char* s2);

// TODO: Insert sub-string and delete range. The list functionality will not
// need to support this, so this library will need to manipulate the raw
// buffer. It may be worth it to put a hook in the lists to swap or replace
// the buffer.
void truncate_string(Str* str, int index);
void clear_string(Str* str);
int length_string(Str* str);
void add_string_Str(Str* ptr, Str* str);
void print_string(FILE* fp, Str* str);
void printf_string(FILE* fp, Str* str, ...);
Str* upcase_string(Str* str);
Str* downcase_string(Str* str);


// TODO: Swap, sort, and find to be implemented mostly in the list functions
// but the compare will have to be implemented in this part.
static inline StrList* create_string_list() {
    return create_list(sizeof(void*));
}

static inline void destroy_string_list(StrList* lst) {
    destroy_list(lst);
}

static inline void add_string_list(StrList* lst, Str* str) {
    append_list(lst, &str);
}

static inline StrListIter* init_string_list_iterator(StrList* lst) {
    return init_list_iterator(lst);
}

static inline Str* iterate_string_list(StrListIter* ptr) {
    Str* val;
    if(iterate_list(ptr, &val))
        return val;
    else
        return NULL;
}

static inline void push_string_list(StrList* lst, Str* str) {
    push_list(lst, &str);
}

static inline Str* peek_string_list(StrList* lst) {
    void* val;
    peek_list(lst, &val);
    return val;
}

static inline Str* pop_string_list(StrList* lst) {
    void* val;
    pop_list(lst, &val);
    return val;
}

//-----------------------------------------------------------------
// hash.c
//-----------------------------------------------------------------

typedef struct {
    const char* key;
    void* data;
    size_t size;
} _hash_node;

/*
 * If a node's key is NULL, but the bucket pointer in the table
 * is not NULL, then the bucket is a tombstone.
 */
typedef struct {
    _hash_node** table;
    int cap;
    int count;
    int tombstones;
} HashTable;

typedef enum {
    HASH_OK,
    HASH_DUP,
    HASH_NF,
} HashResult;

// TODO: use the list functions for the table. That means that the list needs to
// be able to handle NULL pointers.
HashTable* create_hashtable();
void destroy_hashtable(HashTable* table);
HashResult insert_hashtable(HashTable* table, const char* key, void* data, size_t size);
HashResult find_hashtable(HashTable* tab, const char* key, void* data, size_t size);
HashResult remove_hashtable(HashTable* tab, const char* key);

//-------------------------------------------------------------
// fileio.c
//-------------------------------------------------------------
#define END_OF_FILE -1
#define END_OF_INPUT -2

void push_input_file(const char* fname);
void pop_input_file();
int get_char();
int consume_char();

int get_line_no();
int get_col_no();
const char* get_fname();

// Since multiple output files can be open in a moment, then it is accessed
// using an opaque handle.
typedef void* FPTR;
FPTR open_output_file(const char* fname);
void close_output_file(FPTR fp);

void emit_buf(FPTR h, void* buf, unsigned int size);
void emit_fmt(FPTR h, const char* fmt, ...);
void emit_Str(FPTR h, Str* str);
void emit_str(FPTR h, const char* str);

//-------------------------------------------------------------
// cmd.c
//-------------------------------------------------------------
// Flags are a bitmask. For example, OR them together, such as
// (CMD_LIST|CMD_REQD) to specify a required list parameter.
typedef enum {
    CMD_NONE  = 0x00,
    CMD_REQD  = 0x01,
    CMD_LIST  = 0x02,
    CMD_STR   = 0x04,
    CMD_BOOL  = 0x08,
    CMD_FLOAT = 0x10,
    CMD_INT   = 0x20,
    CMD_SEEN  = 0x40,
    CMD_HELP  = 0x80,
} CmdFlag;

// Opaque handle for command line.
typedef void* CmdLine;

// Create the command line data structure.
CmdLine create_cmd_line(const char* description);

// Destroy the command line data structure.
void destroy_cmd_line(CmdLine cl);

// Add a command line parameter to the parser.
//    parm = the neme to recognize on the command line.
//    name = the name to use when retrieving a parameter.
//    dvalue = the default value of the parameter.
//    flags = controls the specify the behavior of the
//            parameter on the command line.
// If this is called with a NULL param and CMD_LIST as a
// flag, then random strings (like file names) will be
// stored under the name.
void add_cmd(CmdLine cl,
             const char* parm,
             const char* name,
             const char* help,
             const char* dvalue,
             unsigned char flags);

// Read the actual command line into the data structure and abort
// the program if there is an error.
void parse_cmd_line(CmdLine cl, int argc, char** argv);

// retrieve the value stored by the name, or publish an error.
Str* get_cmd_str(CmdLine cl, const char* name);
const char* get_cmd_raw(CmdLine cl, const char* name);
long int get_cmd_int(CmdLine cl, const char* name);
unsigned long int get_cmd_unsigned(CmdLine cl, const char* name);
double get_cmd_float(CmdLine cl, const char* name);
StrList* get_cmd_list(CmdLine cl, const char* name);
bool get_cmd_bool(CmdLine cl, const char* name);
CmdFlag get_cmd_flag(CmdLine cl, const char* name);

// Print out the current state of the data structures for debugging.
void dump_cmd_line(CmdLine cl);

//------------------------------------------------------------------------------
//
// except.c
//
//------------------------------------------------------------------------------
// This implementation of exceptions is used to handle errors. THere are no
// classes in C. This uses a global enum to register an exception handler by name.
// Using the non-local goto functionality and the preprocessor of C, a
// reasonably easy to maintain exception capability can be achieved. This has to
// use macros because the current context in the function that handles the
// exceptions is required.

// These data structures should never be accessed directly.
// They need to be defined here because of the macros.
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

//------------------------------------------------------------------------------
//
// trace.c
//
//------------------------------------------------------------------------------
#ifdef USE_TRACE
extern int trace_count;
extern const int trace_increment;
void push_trace_state(bool state);
void set_trace_state(bool state);
bool pop_trace_state();
bool peek_trace_state();

#define PAD printf("%*s", trace_count, "")
//#define CAP printf(" (%d)\n", __LINE__)
#define CAP fputc('\n', stdout);

#define TRACE(f, ...)                          \
    do {                                       \
        if(peek_trace_state()) { \
            PAD;                                   \
            printf("TRACE: %s(): ", __func__);     \
            printf((f) __VA_OPT__(,) __VA_ARGS__); \
            CAP;              \
        } \
    } while(false)

#define ENTER                              \
    do {                                   \
        if(peek_trace_state()) { \
            PAD;                               \
            printf("ENTER: %s(): ", __func__); \
            trace_count += trace_increment;    \
            CAP;                               \
        } \
    } while(false)

#define RET                                 \
    do {                                    \
        if(peek_trace_state()) { \
            trace_count -= trace_increment;     \
            PAD;                                \
            printf("RETURN: %s(): ", __func__); \
            CAP;                                \
        } \
        return;                             \
    } while(false)

#define RETV(v)                                     \
    do {                                            \
        if(peek_trace_state()) { \
            trace_count -= trace_increment;             \
            PAD;                                        \
            printf("RETURN(%s): %s(): (%p)", #v, __func__, (void*)(v)); \
            CAP;                                        \
        } \
        return (v);                                 \
    } while(false)

#define EXIT(n)                               \
    do {                                      \
        if(peek_trace_state()) { \
            PAD;                                  \
            printf("EXIT(#n): %s(): ", __func__); \
            CAP;                                  \
        } \
        exit(v);                              \
    } while(false)

#define PUSH_TRACE_STATE(s) \
    do { \
        push_trace_state(s); \
        TRACE("push trace state: %s", (s)? "true": "false"); \
    } while(false)

#define SET_TRACE_STATE(s) \
    do { \
        set_trace_state(s); \
        TRACE("set trace state: %s", (s)? "true": "false"); \
    } while(false)

#define POP_TRACE_STATE() \
    do { \
        pop_trace_state(); \
        TRACE("pop trace state"); \
    } while(false)

#else

#define TRACE(f, ...)
#define ENTER
#define EXIT(n) exit(n)
#define RET return
#define RETV(v) return (v)
#define PUSH_TRACE_STATE(s)
#define POP_TRACE_STATE()
#define SET_TRACE_STATE(s)

#endif

#endif /* _UTIL_H */
