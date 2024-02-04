/**
 * @file ast.h
 *
 * @brief The AST data structure is simply a hash table. Attributes are
 * stored in it by name.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-15-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _AST_H
#define _AST_H

typedef enum {
    // Terminal symbols
    AST_BREAK = 1000,
    AST_CASE,
    AST_CONTINUE,
    AST_CONST,
    AST_DEFAULT,
    AST_IMPORT,
    AST_DO,
    AST_ELSE,
    AST_FOR,
    AST_IF,
    AST_RETURN,
    AST_SWITCH,
    AST_WHILE,
    AST_IN,
    AST_TO,
    AST_AS,
    AST_YIELD,
    AST_EXIT,
    AST_TRY,
    AST_EXCEPT,
    AST_RAISE,
    AST_CREATE,
    AST_DESTROY,
    AST_START,
    AST_NAMESPACE,
    AST_CLASS,
    AST_STRUCT,
    AST_PUBLIC,
    AST_PRIVATE,
    AST_PROTECTED,
    AST_NUMBER,
    AST_NOTHING,
    AST_STRING,
    AST_BOOLEAN,
    AST_LIST,
    AST_DICT,
    AST_TRACE,
    AST_PRINT,
    AST_TYPE,
    AST_TRUE_BOOL,
    AST_FALSE_BOOL,
    AST_LORE,
    AST_GORE,
    AST_EQU,
    AST_NEQU,
    AST_OR,
    AST_AND,
    AST_NOT,
    AST_ADD_ASSIGN,
    AST_SUB_ASSIGN,
    AST_MUL_ASSIGN,
    AST_DIV_ASSIGN,
    AST_MOD_ASSIGN,
    AST_ADD,
    AST_SUB,
    AST_ASSIGN,
    AST_DIV,
    AST_MUL,
    AST_MOD,
    AST_OPAREN,
    AST_CPAREN,
    AST_OCBRACE,
    AST_CCBRACE,
    AST_OSBRACE,
    AST_CSBRACE,
    AST_COMMA,
    AST_DOT,
    AST_OPBRACE,
    AST_CPBRACE,
    AST_COLON,
    AST_CARAT,
    AST_AMPER,
    AST_INLINE,
    AST_LITERAL_NUM,
    AST_LITERAL_STR,
    AST_SYMBOL,

    // Non-terminal symbols
    AST_scope_operator = 2000,
    AST_type_name,
    AST_type_spec,
    AST_type_spec_element,
    AST_compound_name,
    AST_namespace_element,
    AST_namespace_element_list,
    AST_namespace_definition,
    AST_module_element,
    AST_module_element_list,
    AST_module,

    // For the calls to RAISE();
    AST_GENERIC_ERROR = 3000,
} AstType;

#include "util.h"

typedef HashTable AstNode;
typedef enum {
    AST_OK,
    AST_DUP,
    AST_NF,
} AstResult;

AstNode* create_ast_node(AstType type);
AstResult add_ast_attrib(AstNode* node, const char* key, void* data, size_t size);
AstResult get_ast_attrib(AstNode* name, const char* key, void* data, size_t size);

#endif /* _AST_H */
