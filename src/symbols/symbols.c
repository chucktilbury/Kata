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
#include "memory.h"
#include "hash.h"
#include "symbols.h"

static HashTable* _local_table;

/**
 * @brief Initialize the hash table.
 * 
 */
SymtabResult init_symbol_table() {

    _local_table = create_hashtable();
    return ST_OK;
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

    return (SymtabResult)insert_hashtable(_local_table, key, node);
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

    return find_hashtable(_local_table, key);
}

/**
 * @brief Delete a symbol table entry that 
 * 
 * @param key 
 * @return SymtabResult 
 */
SymtabResult delete_symtab_node(const char* key) {

    return (SymtabResult)remove_hashtable(_local_table, key);
}

void dump_hash_table(HashTable*);
/**
 * @brief Dump the symbol table for debugging.
 * 
 */
void dump_symtab() {

    dump_hash_table(_local_table);
}

