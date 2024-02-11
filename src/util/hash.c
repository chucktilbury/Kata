
/*
 *
 *  https://programming.guide/hash-tables-open-addressing.html
 *
 * The hash table uses linear probing where the probing distance is
 * hash & 0x0F. If the result is 0 then the distance is 1. When a hash
 * is deleted, the memory is freed and the tombstone flag is set. When
 * a hash is added, it can be added to a bucket which is a tombstone.
 *
 * The table is full when 3/4 of the buckets are in use. The table is
 * resized and all of the existing hashes are rehashed into the new table.
 *
 * The add function tracks the max number of hops that are needed to
 * insert a new hash. If the hops exceed a certain number, then the hash
 * table should be rehashed, but only if a certain number of adds have
 * taken place to avoid performance problems.
 *
 * Maybe tombstones should be counted instead of hops, but I do not
 * anticipate needing to delete a lot of entries. Rehashing deletes
 * tombstones.
 *
 * Test build string:
 * clang -Wall -Wextra -DTEST -g -o t hash_examp.c memory.c
 */

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


static uint32_t hash_func(const char* key) {

    uint32_t hash = 2166136261u;
    int slen = strlen(key);

    for(int i = 0; i < slen; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }

    return hash;
}

static int find_slot(HashTable* tab, const char* key) {

    uint32_t hash = hash_func(key) & (tab->cap - 1);
    int inc = hash & 0x0F;
    inc = (inc == 0) ? 1 : inc;

    if(tab->table[hash] == NULL) {
        tab->count++;
        return hash;
    }
    else {
        do {
            for(int i = 0; i < tab->cap; i++) {
                if(tab->table[hash] == NULL) {
                    tab->count++;
                    return hash;
                }
                else if(tab->table[hash]->key == NULL) {
                    tab->tombstones--;
                    tab->count++;
                    return hash;
                }
                else if(strcmp(tab->table[hash]->key, key) == 0) {
                    return hash; // duplicate key
                }
                else
                    hash = (hash + inc) & (tab->cap - 1);
            }
            inc = 1; // slot not found
        } while(true);
    }

    return -1; // keep the compiler happy
}

static void rehash_table(HashTable* tab) {

    if(tab->count * 1.75 > tab->cap) {
        int oldcap = tab->cap;
        _hash_node** oldtab = tab->table;
        tab->cap <<= 1; // double the capacity
        tab->tombstones = 0;
        tab->count = 0;
        tab->table = _ALLOC_ARRAY(_hash_node*, tab->cap);
        for(int i = 0; i < tab->cap; i++)
            tab->table[i] = NULL;

        int slot;

        for(int i = 0; i < oldcap; i++) {
            if(oldtab[i] != NULL && oldtab[i]->key != NULL) {
                slot = find_slot(tab, oldtab[i]->key);
                tab->table[slot] = oldtab[i];
            }
        }
        _FREE(oldtab);
    }
}

HashTable* create_hashtable() {

    HashTable* tab = _ALLOC_T(HashTable);

    tab->count = 0;
    tab->cap = 0x01 << 3;

    tab->table = _ALLOC_ARRAY(_hash_node*, tab->cap);
    for(int i = 0; i < tab->cap; i++)
        tab->table[i] = NULL;

    return tab;
}

void destroy_hashtable(HashTable* table) {

    if(table != NULL) {
        for(int i = 0; i < table->cap; i++) {
            if(table->table[i] != NULL) {
                if(table->table[i]->key != NULL) {
                    _FREE(table->table[i]->key);
                    _FREE(table->table[i]->data);
                }
            }
            _FREE(table->table[i]);
        }

        _FREE(table->table);
        _FREE(table);
    }
}

HashResult insert_hashtable(HashTable* table, const char* key, void* data, size_t size) {

    rehash_table(table);

    int slot = find_slot(table, key);
    if(slot < 0)
        return false;

    // help me, obi wan optimizer, you are my only hope
    if(table->table[slot] != NULL) {
        if(table->table[slot]->key != NULL) {
            // printf("cannot store duplicate key: \"%s\"\n", key);
            return HASH_DUP;
        }
    }
    else
        table->table[slot] = _ALLOC_T(_hash_node);

    table->table[slot]->key = _DUP_STR(key);
    if(data != NULL && size != 0) {
        table->table[slot]->data = _ALLOC(size);
        table->table[slot]->size = size;
        memcpy(table->table[slot]->data, data, size);
    }
    else {
        table->table[slot]->data = NULL;
        table->table[slot]->size = 0;
    }

    return HASH_OK;
}

HashResult find_hashtable(HashTable* tab, const char* key, void* data, size_t size) {

    int slot = find_slot(tab, key);

    if(tab->table[slot] != NULL && tab->table[slot]->key != NULL) {
        if(strcmp(tab->table[slot]->key, key) == 0) {
            if(tab->table[slot]->size != size)
                printf("data size mismatch: %lu != %lu\n", size,
                       tab->table[slot]->size);
            memcpy(data, tab->table[slot]->data, size);
            return HASH_OK;
        }
    }

    return HASH_NF;
}

HashResult remove_hashtable(HashTable* tab, const char* key) {

    int slot = find_slot(tab, key);

    if((tab->table[slot] != NULL) && (tab->table[slot]->key != NULL)) {
        if(strcmp(tab->table[slot]->key, key) == 0) {
            _FREE(tab->table[slot]->data);
            _FREE(tab->table[slot]->key);
            tab->table[slot]->key = NULL;
            tab->table[slot]->data = NULL;
            tab->table[slot]->size = 0;
            tab->count--;
            tab->tombstones++;
            return HASH_OK;
        }
    }

    return HASH_NF;
}
