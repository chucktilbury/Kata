/**
 * @file ast.c
 *
 * @brief Implement the AST node functionality.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-15-2024
 * @copyright Copyright (c) 2024
 */
#include "ast.h"

/**
 * @brief Create a ast node object
 *
 * @param type
 * @return AstNode*
 *
 */
AstNode* create_ast_node(AstType type) {

    AstNode* node = create_hashtable();
    HashResult res = insert_hashtable(node, "type", &type, sizeof(AstType));
    if(res != HASH_OK)
        RAISE(AST_GENERIC_ERROR, "cannot create AST table entry");

    return node;
}

/**
 * @brief Add an AST attribute to the node. Data is duplicated and copied to
 * the hash table data structure.
 *
 * @param node
 * @param key
 * @param data
 * @param size
 *
 */
AstResult add_ast_attrib(AstNode* node, const char* key, void* data, size_t size) {

    AstResult res = (AstResult)insert_hashtable(node, key, data, size);
    if(res != AST_OK)
        RAISE(AST_GENERIC_ERROR, "cannot add AST table attribute");

    return res;
}

/**
 * @brief Return the AST attribute object as a void pointer. The caller will
 * need to provide a buffer and a size to recover the data from the table.
 *
 * @param name
 * @param key
 * @param data
 * @param size
 *
 */
AstResult get_ast_attrib(AstNode* node, const char* key, void* data, size_t size) {

    return (AstResult)find_hashtable(node, key, data, size);
}
