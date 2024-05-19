/**
 * @file symbols.h
 * 
 * @brief 
 * 
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-05-14
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SYMBOLS_H_
#define _SYMBOLS_H_

#include "hash.h"
#include "context.h"

typedef enum {
    ST_OK = HASH_OK,
    ST_DUP = HASH_DUP,
    ST_NF = HASH_NF,
} SymtabResult;

// actual list entry
typedef struct _symtab_node_ {
    const char* key;        // this is taken from the context
    void* ast_node;         // the AST node that this points to
    SymContext* context;    // full context of this table entry
} SymtabNode;

SymtabResult init_symbol_table();
SymtabResult create_symtab_node(const char* key, void* node);
void* find_symtab_node(const char* key);
SymtabResult delete_symtab_node(const char* key);

#endif  /* _SYMBOLS_H_ */
