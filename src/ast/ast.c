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
#include "internal_parser.h"
#include "scanner.h"
#include "trace.h"

/**
 * @brief Create a ast node object
 *
 * @param type
 * @return AstNode*
 *
 */
AstNode* create_ast_node(AstType type, size_t size) {

    AstNode* node  = create_hashtable();
    HashResult res = insert_hashtable(node, "type", &type, sizeof(AstType));
    if(res != HASH_OK)
        RAISE(AST_ERROR, "cannot create AST table entry");

    return node;
}

/**
 * @brief Add an AST attribute to the node. Data is duplicated and copied
 * to the hash table data structure.
 *
 * @param node
 * @param key
 * @param data
 * @param size
 *
 */
AstResult add_ast_attrib(AstNode* node, const char* key, void* data, size_t size) {

    AstResult res = (AstResult)insert_hashtable(node, key, data, size);
    if(res == AST_DUP)
        RAISE(AST_DUP_ERROR, "AST table attribute \"%s\" is a duplicate", key);
    else if(res != AST_OK)
        RAISE(AST_ERROR, "cannot add AST table attribute");

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

static const char* n_to_str(AstType type) {

    return (type == AST_scope_operator)          ? "scope_operator" :
            (type == AST_type_name)              ? "type_name" :
            (type == AST_type_spec)              ? "type_spec" :
            (type == AST_type_spec_element)      ? "type_spec_element" :
            (type == AST_compound_name)          ? "compound_name" :
            (type == AST_namespace_element)      ? "namespace_element" :
            (type == AST_namespace_element_list) ? "namespace_element_list" :
            (type == AST_namespace_definition)   ? "namespace_definition" :
            (type == AST_module_element)         ? "module_element" :
            (type == AST_module_element_list)    ? "module_element_list" :
            (type == AST_import_statement)       ? "import_statement" :
            (type == AST_module)                 ? "module" :
                                                   "UNKNOWN";
}

/**
 * @brief Traverse the tree and print out information about what is in the
 * tree and where it is.
 *
 * @param node
 *
 */
void traverse_ast(AstNode* tree) {

    ENTER;
    Token tok;
    AstNode node;
    AstType type;
    List list;

    AstResult res = get_ast_attrib(tree, "type", &type, sizeof(AstType));
    if(AST_NF == res)
        RAISE(AST_NF_ERROR, "ast node does not have a \"type\" entry: %p\n", (void*)tree);
    else if(AST_OK != res)
        RAISE(AST_ERROR, "cannot get an AST \"type\" entry: %p\n", (void*)tree);

    TRACE("type: \"%s\" (%d)", n_to_str(type), type);

    if(AST_OK == get_ast_attrib(tree, "token", &tok, sizeof(Token))) {
        TRACE_TERM(&tok);
    }

    if(AST_OK == get_ast_attrib(tree, "nterm", &node, sizeof(AstNode))) {
        traverse_ast(&node);
    }

    if(AST_OK == get_ast_attrib(tree, "tlist", &list, sizeof(List))) {
        TRACE("tlist length: %d", length_list(&list));
        ListIter* iter = init_list_iterator(&list);
        Token tok;
        while(iterate_list(iter, &tok)) {
            TRACE_TERM(&tok);
        }
    }

    if(AST_OK == get_ast_attrib(tree, "list", &list, sizeof(List))) {
        TRACE("list length: %d", length_list(&list));
        ListIter* iter = init_list_iterator(&list);
        AstNode lnode;
        while(iterate_list(iter, &lnode)) {
            traverse_ast(&lnode);
        }
    }

    RET;
}

/**
 * @brief Print the non-terminal symbol for the trace functionality.
 *
 * @param node
 *
 */
void print_nonterminal(AstNode* node) {

    AstType type;
    List lst;
    Token tok;
    AstNode nterm;

    if(AST_OK == get_ast_attrib(node, "type", &type, sizeof(AstType)))
        printf("non-terminal: %s ", n_to_str(type));

    if(AST_OK == get_ast_attrib(node, "list", &lst, sizeof(List)))
        printf("as \"list\" ");

    if(AST_OK == get_ast_attrib(node, "nterm", &nterm, sizeof(AstNode)))
        printf("as \"nterm\" ");

    if(AST_OK == get_ast_attrib(node, "token", &tok, sizeof(Token)))
        print_token(&tok);
}
