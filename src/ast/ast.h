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
    // module.c
    AST_symbol = 2000,
    AST_scope_operator,
    AST_compound_name,
    AST_module_element,
    AST_module_element_list,
    AST_module,

    // namespace.c
    AST_namespace_element,
    AST_namespace_element_list,
    AST_namespace_definition,

    // data.c
    AST_type_name,
    AST_type_spec,
    AST_type_spec_element,

    // import.c
    AST_import_statement,

    // expresion.c
    AST_primary_expression,
    AST_expression_list,
    AST_expression,

} AstType;

#include "util.h"

typedef struct {
    AstType type;
} AstNode;

//typedef HashTable AstNode;
typedef enum {
    AST_OK,
    AST_DUP,
    AST_NF,
} AstResult;

#define CREATE_AST_NODE(d, t)   create_ast_node((t), sizeof(d))

AstNode* create_ast_node(AstType type, size_t size);
AstResult add_ast_attrib(AstNode* node, const char* key, void* data, size_t size);
AstResult get_ast_attrib(AstNode* name, const char* key, void* data, size_t size);
void traverse_ast(AstNode* node);
void print_nonterminal(AstNode* node);

// module.c
typedef struct __ast_module__ AstModule;
typedef struct __ast_scope_operator__ AstScopeOperator;
typedef struct __ast_module_element__ AstModuleElement;
typedef struct __ast_compound_name__ AstCompoundName;

// namespace.c
typedef struct __ast_namespace_element__ AstNamespaceElement;
typedef struct __ast_namespace_element_list__ AstNamespaceElementList;
typedef struct __ast_namespace_definition__ AstNamespaceDefinition;

// data.c
typedef struct __ast_type_name__ AstTypeName;
typedef struct __ast_type_spec_element__ AstTypeSpecElement;
typedef struct __ast_type_spec__ AstTypeSpec;

// import.c
typedef struct __ast_import_statement__ AstImportStatement;

// expression.c
typedef struct __ast_primary_expression__ AstPrimaryExpression;
typedef struct __ast_expression_list__ AstExpressionList;
typedef struct __ast_expression__ AstExpression;
typedef struct __ast_cast_statement__ AstCastExpression;

#endif /* _AST_H */
