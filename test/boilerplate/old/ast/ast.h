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
#include "util.h"
#include "scanner.h"

typedef enum {
    // module.c
    AST_module = 0x2030,
    AST_scope_operator,
    AST_compound_name,
    AST_module_element,
    AST_module_element_list,

    // namespace.c
    AST_namespace_element,
    AST_namespace_body,
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
    AST_cast_expression,

    // string.c
    AST_formatted_string,

} AstType;

typedef enum {
    AST_TRAVERSE_ERROR = 0x8020,
} AstError;

typedef struct {
    AstType type;
    bool seen;
} AstNode;

// module.c
typedef struct __ast_module__ {
    AstNode node;
    PtrList* lst; // list of AstNode*'s
} AstModule;

typedef struct __ast_scope_operator__ {
    AstNode node;
    Token* tok; // single token
} AstScopeOperator;

typedef struct __ast_module_element__ {
    AstNode node;
    AstNode* elem; // a namespace or import definition
} AstModuleElement;

typedef struct __ast_compound_name__ {
    AstNode node;
    PtrList* lst; // list of tokens
    Str* str; // raw string
} AstCompoundName;

// namespace.c
typedef struct __ast_namespace_element__ {
    AstNode node;
    AstNode* elem; // one of several types
} AstNamespaceElement;

typedef struct __ast_namespace_body__ {
    AstNode node;
    PtrList* lst; // list of body elements
} AstNamespaceBody;

typedef struct __ast_namespace_definition__ {
    AstNode node;
    Token* name;
    struct __ast_namespace_body__* body;
} AstNamespaceDefinition;

// data.c
typedef struct __ast_type_name__ {
    AstNode node;
    Token* tok; // a single non-terminal
} AstTypeName;

typedef struct __ast_type_spec_element__ {
    AstNode node;
    AstNode* elem; // could be a type name or a compound name
} AstTypeSpecElement;

typedef struct __ast_type_spec__ {
    AstNode node;
    bool is_const;
    struct __ast_type_spec_element__* elem;
} AstTypeSpec;

// import.c
typedef struct __ast_import_statement__ {
    AstNode node;
    struct __ast_formatted_string__* fname;
    Token* symbol;
    struct __ast_module__* module;
} AstImportStatement;

// expression.c
typedef struct __ast_primary_expression__ {
    AstNode node;
    Token* num;     // literal number
    AstNode* elem;  // other non-terminals
} AstPrimaryExpression;

typedef struct __ast_expression_list__ {
    AstNode node;
    PtrList* lst;
} AstExpressionList;

typedef struct __ast_expression__ {
    AstNode node;
} AstExpression;

typedef struct __ast_cast_expression__ {
    AstNode node;
} AstCastExpression;

typedef struct __ast_formatted_string__ {
    AstNode node;
    Token* strg;
    struct __ast_expression_list__* expr_lst;
} AstFormattedString;

#define CREATE_AST_NODE(d, t)   (t*)create_ast_node((d), sizeof(t))

AstNode* create_ast_node(AstType type, size_t size);
AstType get_ast_node_type(AstNode* node);
void traverse_ast(AstModule* node);
void print_nonterminal(AstNode* node);
const char* n_to_str(AstNode* node);

#endif /* _AST_H */
