#ifndef SYMBOLS_H
#define SYMBOLS_H

typedef enum {
    AT_ERROR,       // errors...
    AT_ROOT,        // root node type

    AT_OBJ_NAME,    // string name of object
    AT_OBJ_TYPE,    // int property type of this node.
    AT_CLASS,       // int properties
    AT_DATA_DECL,
    AT_FUNC_DECL,
    AT_OBJ_PTR,     // if there is a pointer, this will be it

    AT_FILE_NAME,   // string property from scanner
    AT_LINE_NO,     // int property from scanner
    AT_COL_NO,      // int property from scanner

    AT_PARENT_PTR,  // pointer property
    AT_PARENT_NAME, // string property
    AT_INHERIT_PTR, // pointer property
    AT_INHERIT_NAME,// string property

    AT_SCOPE,       // int property
    AT_PUBLIC,
    AT_PRIVATE,
    AT_PROTECTED,

    AT_RETURN_TYPE, // int property
    AT_FLOAT_TYPE,
    AT_INT_TYPE,
    AT_UINT_TYPE,
    AT_BOOL_TYPE,
    AT_DICT_TYPE,
    AT_LIST_TYPE,
    AT_STRING_TYPE,
    AT_ANY_TYPE,
    AT_USER_TYPE,

    AT_USER_TYPE_PTR,   // pointer to the tree symbol of the user type
    AT_USER_DATA_PTR,   // pointer to the token payload, if any
    AT_USER_TYPE_NAME,  // string property

    AT_CHILD_PTR,   // pointer to a tree that holds the children of this object.

} attr_type_t;

typedef struct _attrs {
    attr_type_t type;
    void* data;
    struct _attrs* left;
    struct _attrs* right;
} attrib_t;

typedef struct _ast_node {
    const char* name;
    attrib_t* attrs;
    struct _ast_node* parent;
    struct _ast_node* children;
    struct _ast_node* left;
    struct _ast_node* right;
} ast_t;

ast_t* create_root_node();
ast_t* create_node(ast_t* parent);

void add_child_node(ast_t* parent, ast_t* node);
void add_node(ast_t* sibling, ast_t* node);
ast_t* find_node(ast_t* node, const char* name);

void set_node_name(ast_t* node, const char* name);
void set_node_attr(ast_t* node, attr_type_t type, void* attr, size_t size);
void set_node_attr_str(ast_t* node, attr_type_t type, const char* attr);
void set_node_attr_int(ast_t* node, attr_type_t type, int var);
void set_node_attr_ptr(ast_t* node, attr_type_t type, void* var);
void set_node_scanner_attrs(ast_t* node);
bool get_node_attr(ast_t* node, attr_type_t type, void** data);

const char* get_node_name(ast_t* node);

attr_type_t tok_to_node_type(token_type t);
ast_t* validate_name(ast_t* node, const char* name);

void dump_tree(ast_t* root);
char const* attr_type_to_str(attr_type_t type);

#endif
