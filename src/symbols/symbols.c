/**
 * @file symbols.c
 *
 * @brief
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-05-14
 * @copyright Copyright (c) 2024
 *
 */
#include "symbols.h"
#include "hash.h"
#include "memory.h"
#include "trace.h"

static HashTable* _local_table;

/**
 * @brief Initialize the hash table.
 *
 */
SymtabResult init_symbol_table() {

    ENTER;
    _local_table = create_hashtable();
    RETV(ST_OK);
}

/**
 * @brief Create a symbol table node object
 *
 * @param key
 * @param type
 * @param scope
 * @param ast_node
 * @return Node*
 */
SymtabResult create_symtab_node(const char* key, void* node) {

    ENTER;
    SymtabResult retv = (SymtabResult)insert_hashtable(_local_table, key, node);
    RETV(retv);
}

/**
 * @brief Find the node based on the provided key. If the result pointer
 * is not NULL, then the result of the hash table find will be placed there.
 * If the symbol is not found, then the return value is NULL.
 *
 * @param key
 * @param result
 * @return SymtabNode*
 */
void* find_symtab_node(const char* key) {

    ENTER;
    void* retv = find_hashtable(_local_table, key);
    RETV(retv);
}

/**
 * @brief Delete a symbol table entry that
 *
 * @param key
 * @return SymtabResult
 */
SymtabResult delete_symtab_node(const char* key) {

    ENTER;
    SymtabResult retv = (SymtabResult)remove_hashtable(_local_table, key);
    RETV(retv);
}

void dump_hash_table(HashTable*);
/**
 * @brief Dump the symbol table for debugging.
 *
 */
void dump_symtab() {

    ENTER;
    dump_hash_table(_local_table);
    RET;
}
