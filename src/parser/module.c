/**
 * @file parser1.c
 *
 * @brief This is the bottom most level where the most simple objects are
 * parsed that are used many places.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-13-2024
 * @copyright Copyright (c) 2024
 */
#include "util.h"
#include "scanner.h"
#include "parser.h"
#include "errors.h"

/**
 * @brief This looks for a single keyword and returns the token associated
 * with it. This can only be used in conjunction with other rules, since it
 * can only return a terminal. Therefore, it cannot return an error.
 *  TOK_PUBLIC,
 *  TOK_PRIVATE,
 *  TOK_PROTECTED,
 *
 * @return NonTerm*
 *
 */
AstNode* scope_operator() {

    Token* tok = get_token();
    if(tok->type == TOK_PUBLIC ||
            tok->type == TOK_PRIVATE ||
            tok->type == TOK_PROTECTED) {

        AstNode* node = create_ast_node(AST_scope_operator);
        int data = tok->type;
        add_ast_attrib(node, "data", &data, sizeof(data));
        advance_token();
        return node;
    }
    else {
        reset_token_queue();
        return NULL;
    }
}

/**
 * @brief Single keyword that represents the name of a built-in type.
 *  TOK_NUMBER,
 *  TOK_NOTHING,
 *  TOK_STRING,
 *  TOK_BOOLEAN,
 *  TOK_LIST,
 *  TOK_DICT,
 *
 * @return NonTerm*
 *
 */
AstNode* type_name() {

    Token* tok = get_token();
    if(tok->type == TOK_NUMBER ||
            tok->type == TOK_STRING ||
            tok->type == TOK_NOTHING ||
            tok->type == TOK_BOOLEAN ||
            tok->type == TOK_LIST ||
            tok->type == TOK_DICT) {

        AstNode* node = create_ast_node(AST_type_name);
        int data = tok->type;
        add_ast_attrib(node, "data", &data, sizeof(data));
        advance_token();
        return node;
    }
    else {
        reset_token_queue();
        return NULL;
    }
}

/**
 * @brief Parse a compound name. A compound name has the format of
 * SYMBOL ('.' SYMBOL)*
 *
 * @return NonTerm*
 *
 */
AstNode* compound_name() {

    Token* tok = get_token();

    if(tok->type == TOK_SYMBOL) {
        StrList* lst = create_string_list();
        add_string_list(lst, tok->str);
        tok = advance_token();
        if(tok->type == TOK_DOT) {
            // more than one element
            while(true) {
                tok = advance_token();
                if(tok->type == TOK_SYMBOL) {
                    // another element to add
                    add_string_list(lst, tok->str);
                }
                else {
                    // handle an error because a dot must be followed by
                    // a TOK_SYMBOL in this context.
                    handle_error("expected a symbol but got a %s", raw_string(tok->str));
                    return NULL;
                }

                tok = advance_token();
                if(tok->type != TOK_DOT) {
                    // non-terminal is complete
                    AstNode* node = create_ast_node(AST_compound_name);
                    add_ast_attrib(node, "data", lst, sizeof(StrList));
                    advance_token();
                    return node;
                }
            }
        }
        else {
            // single symbol is a match
            AstNode* node = create_ast_node(AST_compound_name);
            add_ast_attrib(node, "data", lst, sizeof(StrList));
            advance_token();
            return node;
        }
    }
    else {
        // not a match
        reset_token_queue();
        return NULL;
    }
}

/**
 * @brief A type spec element is a component of a complete type specification.
 *      type_spec_element
 *          : type_name
 *          | compound_name
 *
 * @return NonTerm*
 *
 */
AstNode* type_spec_element() {

    AstNode* node;

    if((NULL != (node = type_name())) || (NULL != (node = compound_name()))) {
        // the element is a valid type name
        AstNode* node = create_ast_node(AST_type_spec_element);
        add_ast_attrib(node, "data", node, sizeof(AstNode));
        advance_token();
        return node;
    }
    else {
        // this is not a match, but it could match the next alternative in
        // the caller.
        reset_token_queue();
    }

    // there is no match, but that is not an error, yet.
    return NULL;
}

/**
 * @brief A type spec is a complete specification to name a type.
 *      type_spec
 *          : CONST type_spec_element
 *          | type_spec_element
 *
 * @return NonTerm*
 *
 */
AstNode* type_spec() {

    AstNode* nt;
    Token* tok = get_token();
    bool constant = false;

    if(tok->type == TOK_CONST) {
        advance_token();
        if(NULL != (nt = type_spec_element())) {
            constant = true;
            AstNode* node = create_ast_node(AST_type_spec);
            add_ast_attrib(node, "is_const", &constant, sizeof(bool));
            add_ast_attrib(node, "data", nt, sizeof(AstNode));
            return node;
        }
        else {
            handle_error("expected a type name specification but got a %s", raw_string(tok->str));
            return NULL;
        }
    }
    else if(NULL != (nt = type_spec_element())) {
        AstNode* node = create_ast_node(AST_type_spec);
        add_ast_attrib(node, "is_const", &constant, sizeof(bool));
        add_ast_attrib(node, "data", nt, sizeof(AstNode));
        return node;
    }
    else {
        // not an error ... yet.
        reset_token_queue();
        return NULL;
    }

    return NULL;
}

// forward declaration
AstNode* namespace_definition();
/**
 * @brief An element that can be in a namespace.
 *      namespace_element
 *          : namespace_definition
 *          | type_spec
 *          | scope_operator
 *
 * @return AstNode*
 *
 */
AstNode* namespace_element() {

    AstNode* node;

    if((NULL != (node = namespace_definition())) ||
            (NULL != (node = type_spec())) ||
            (NULL != (node = scope_operator()))) {

        AstNode* nnode = create_ast_node(AST_namespace_element);
        add_ast_attrib(nnode, "data", node, sizeof(AstNode));
        return nnode;
    }
    else
        reset_token_queue();

    // not found, not an error
    return NULL;
}

/**
 * @brief List of namespace elements.
 *      namespace_elem_list
 *          : namespace_element (namespace_element)*
 *
 * @return AstNode*
 *
 */
AstNode* namespace_element_list() {

    AstNode* node;
    // the GC will discard this if it is not returned
    List* lst = create_list(sizeof(AstNode));

    while(NULL != (node = namespace_element()))
        append_list(lst, (void*)node);

    if(length_list(lst) > 0) {
        AstNode* nnode = create_ast_node(AST_namespace_element_list);
        add_ast_attrib(nnode, "data", lst, sizeof(List));
        return nnode;
    }
    else {
        reset_token_queue();
    }

    // empty list is permissable
    return NULL;
}

/**
 * @brief A complete namespace specification.
 *      namespace_definition
 *          : NAMESPACE SYMBOL '{' '}'
 *          | NAMESPACE SYMBOL '{' namespace_elem_list '}'
 *
 * @return AstNode*
 *
 */
AstNode* namespace_definition() {

    AstNode* node, *nnode;
    Token* tok = get_token();

    if(tok->type == TOK_NAMESPACE) {
        tok = advance_token();
        if(tok->type == TOK_SYMBOL) {
            nnode = create_ast_node(AST_namespace_definition);
            add_ast_attrib(nnode, "name", tok->str, sizeof(Str));
        }
        else {
            // namespace requires a name, so this is an error
            handle_error("namespace expects a name, but got a %s", raw_string(tok->str));
            return NULL;
        }

        tok = advance_token();
        if(tok->type != TOK_OCBRACE) {
            // namespace requires a '{', so this is an error
            handle_error("expected a '{', but got a %s", raw_string(tok->str));
            return NULL;
        }

        if(NULL != (node = namespace_element_list())) {
            add_ast_attrib(nnode, "data", node, sizeof(AstNode));
        }
        // else not an error, the data attribute does not exist

        tok = advance_token();
        if(tok->type != TOK_OCBRACE) {
            // namespace requires a '}', so this is an error
            handle_error("expected a '}', but got a %s", raw_string(tok->str));
            return NULL;
        }
    }
    else {
        // not a namespace, not an error
        reset_token_queue();
        return NULL;
    }

    // cannot happen. make the compiler happy
    return NULL;
}

#if 0
/**
 * @brief Import statement allows the compiler to switch files. This keyword
 * generates a new namespace, but it also controls the scanner directly.
 *      import_statement
 *          : IMPORT LITERAL_STR
 *          | IMPORT LITERAL_STR AS SYMBOL
 *
 * @return AstNode*
 *
 */
AstNode* import_statement() {

    Token* tok = get_token();

    if(tok->type == TOK_IMPORT) {
        tok = advance_token();
        if(tok->type == TOK_LITERAL_STR) {

            tok = advance_token();
        }
        else {
            // error. import requires a string
        }
    }

    // not an import, not an error
    return NULL;
}
#endif

/**
 * @brief Elements that can be in a node are similar to the elements that
 * be in a namespace.
 *      module_element
 *          : scope_operator
 *          | type_spec
 *          | namespace_definition
 *
 * @return AstNode*
 *
 */
AstNode* module_element() {

    AstNode* node;

    if((NULL != (node = scope_operator())) ||
            (NULL != (node = type_spec())) ||
            (NULL != (node = namespace_definition()))) {

        AstNode* nnode = create_ast_node(AST_module_element);
        add_ast_attrib(nnode, "data", node, sizeof(AstNode));
        return nnode;
    }
    else
        reset_token_queue();

    // not found, not an error
    return NULL;
}

/**
 * @brief List of module elements
 *      module_elem_list
 *          : module_element (module_element)*
 *
 * @return AstNode*
 *
 */
AstNode* module_element_list() {

    AstNode* node;
    // the GC will discard this if it is not returned
    List* lst = create_list(sizeof(AstNode));

    while(NULL != (node = module_element()))
        append_list(lst, (void*)node);

    if(length_list(lst) > 0) {
        AstNode* nnode = create_ast_node(AST_module_element_list);
        add_ast_attrib(nnode, "data", lst, sizeof(List));
        return nnode;
    }
    else {
        reset_token_queue();
    }

    // empty list is permissable
    return NULL;
}

/**
 * @brief Top level entry point into the parser. Node returned is a AST_module.
 *      module
 *          : module_elem_list
 *
 * @return AstNode*
 *
 */
AstNode* module() {

    AstNode* node;

    if(NULL != (node = module_element_list())) {
        AstNode* nnode = create_ast_node(AST_module);
        add_ast_attrib(nnode, "data", nnode, sizeof(AstNode));
        return nnode;
    }

    return NULL;
}
