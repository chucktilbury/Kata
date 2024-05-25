/**
 * @file hash.c
 *
 * @brief This is a very simple open-addressing hash table. When a hash
 * collision happens, the next items are checked one at a time for a free
 * slot. When the table is 3/4 full, then it is resized and the table is
 * re-hashed. When a table entry is deleted, then the key is freed, making
 * the slot available for use. The size of the table is never reduced. A
 * single pointer is stored as the node payload and that pointer is never
 * written or freed. The payload is what gets returned when the find method
 * is called.
 *
 *  https://programming.guide/hash-tables-open-addressing.html
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-05-14
 * @copyright Copyright (c) 2024
 *
 */
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "symbols.h"

/**
 * @brief Simple hash function. Do not mess around with the constant values.
 *
 * @param key
 * @return uint32_t
 */
static uint32_t hash_func(const char* key) {

    uint32_t hash = 2166136261u;
    int slen      = strlen(key);

    for(int i = 0; i < slen; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }

    return hash;
}

/**
 * @brief Find the slot in the has table accounting for tombstones and
 * collisions. If ts is set, then the has was deleted and open for a new
 * entry. If the key is NULL, then the node is unallocated.
 *
 * @param tab
 * @param key
 * @return int
 */
static int find_slot(HashTable* tab, const char* key) {

    uint32_t hash = hash_func(key) & (tab->cap - 1);
    for(int i = 0; i < tab->cap; i++) {
        if(tab->table[hash].key == NULL)
            return hash;
        else if(strcmp(tab->table[hash].key, key) == 0)
            return hash; // found key
        else
            hash = (hash + 1) & (tab->cap - 1);
    }

    return -1; // should never happen
}

/**
 * @brief If the table needs to be expanded, then this function will allocate
 * the memory and place all of the entries into the new table.
 *
 * @param tab
 */
static void rehash_table(HashTable* tab) {

    if(tab->count * 1.75 > tab->cap) {
        int oldcap       = tab->cap;
        HashNode* oldtab = tab->table;
        tab->cap <<= 1; // double the capacity
        tab->table = _ALLOC_ARRAY(HashNode, tab->cap);
        for(int i = 0; i < tab->cap; i++)
            memset(&tab->table[i], 0, sizeof(HashNode));

        int slot;
        for(int i = 0; i < oldcap; i++) {
            if(oldtab[i].key != NULL) {
                slot = find_slot(tab, oldtab[i].key);
                // printf("rehash slot: %d: %s\n", slot, oldtab[i].key);
                tab->table[slot].key  = oldtab[i].key;
                tab->table[slot].data = oldtab[i].data;
            }
        }
        _FREE(oldtab);
    }
}

/**
 * @brief debugging function
 *
 * @param tab
 */
void dump_hash_table(HashTable* tab) {

    assert(tab != NULL);

    printf("\ncapacity: %d\n", tab->cap);
    printf("count: %d\n", tab->count);
    for(int i = 0; i < tab->cap; i++) {
        if(tab->table[i].key != NULL) {
            printf("(%d) key: %s\n", i, tab->table[i].key);
        }
    }
    printf("\n");
}

/******************************************************************************
 * PUBLIC INTERFACE
 */

/**
 * @brief Create a hashtable object
 *
 * @return HashTable*
 */
HashTable* create_hashtable() {

    HashTable* tab = _ALLOC_T(HashTable);

    tab->count = 0;
    tab->cap   = 0x01 << 3;

    tab->table = _ALLOC_ARRAY(HashNode, tab->cap);
    for(int i = 0; i < tab->cap; i++)
        memset(&tab->table[i], 0, sizeof(HashNode));

    return tab;
}

/**
 * @brief Destroy the hash table.
 *
 * @param table
 */
void destroy_hashtable(HashTable* table) {

    if(table != NULL) {
        // table being NULL is not an error
        for(int i = 0; i < table->cap; i++) {
            if(table->table[i].data != NULL) {
                if(table->table[i].key != NULL) {
                    _FREE(table->table[i].key);
                }
            }
        }

        _FREE(table->table);
        _FREE(table);
    }
}

/**
 * @brief Insert a Node into the table based on the key in the node.
 *
 * @param table
 * @param key
 * @param data
 * @param size
 * @return HashResult
 */
HashResult insert_hashtable(HashTable* table, const char* key, void* node) {

    assert(table != NULL);
    assert(node != NULL);
    assert(key != NULL);

    rehash_table(table);
    int slot = find_slot(table, key);
    // printf("insert slot: %d: %s\n", slot, key);
    assert(slot >= 0);

    if(table->table[slot].key != NULL)
        return HASH_DUP;
    else {
        table->table[slot].key  = _DUP_STR(key);
        table->table[slot].data = node;
        table->count++;
    }

    return HASH_OK;
}

/**
 * @brief Copy the node data into the pointer, found by the key.
 *
 * @param tab
 * @param key
 * @param data
 * @return HashResult
 */
void* find_hashtable(HashTable* tab, const char* key) {

    assert(tab != NULL);
    assert(key != NULL);

    int slot = find_slot(tab, key);

    // printf("find slot: %d: %s\n", slot, key);
    if(tab->table[slot].key != NULL) {
        if(strcmp(tab->table[slot].key, key) == 0) {
            return tab->table[slot].data;
        }
    }

    return NULL;
}

/**
 * @brief Remove a node from the table based on the key.
 *
 * @param tab
 * @param key
 * @return HashResult
 */
HashResult remove_hashtable(HashTable* tab, const char* key) {

    assert(tab != NULL);
    assert(key != NULL);

    int slot = find_slot(tab, key);
    // printf("remove slot: %d: %s\n", slot, key);

    if(tab->table[slot].key != NULL) {
        if(strcmp(tab->table[slot].key, key) == 0) {
            _FREE(tab->table[slot].key);
            tab->table[slot].key = NULL;
            tab->count--;
            return HASH_OK;
        }
    }

    return HASH_NF;
}
