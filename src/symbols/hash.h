/**
 * @file hash.h
 * 
 * @brief 
 * 
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-05-14
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _HASH_H_
#define _HASH_H_

#include <stdbool.h>
#include <stdlib.h>

/*
 * This is the actual data structure that is stored in the hash table.
 */
typedef struct {
    const char* key;
    void* data;
} HashNode;

/*
 * If a node's key is NULL, but the bucket pointer in the table
 * is not NULL, then the bucket is a tombstone.
 */
typedef struct {
    HashNode* table;
    int cap;
    int count;
} HashTable;

typedef enum {
    HASH_OK,
    HASH_DUP,
    HASH_NF,
} HashResult;

HashTable* create_hashtable();
void destroy_hashtable(HashTable* table);
HashResult insert_hashtable(HashTable* table, const char* key, void* data);
void* find_hashtable(HashTable* tab, const char* key);
HashResult remove_hashtable(HashTable* tab, const char* key);

#endif  /* _HASH_H_ */
