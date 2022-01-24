/*
 * There is a single global symbol table. It stores names in decorated format.
 *
 * Symbols have an attribute table that carries all of the symbol attributes in
 * a tree that has a named format. Every attribute has a name and a generic
 * void* data element that is connected to it. The entity that wants to use the
 * attribute name has to know what to do with the pointer.
 *
 * Symbols and attributes are stored as an unbalanced tree.
 *
 */
#include "common.h"

/*
 * Create the root node. All other nodes are children of this one.
 *
 * Note that the root node will have all methods for native types and all
 * global objects. It does not need to have any attributes.
 */
ast_t* create_root_node() {

    ast_t* node = create_node(NULL);
    set_node_attr_int(node, AT_OBJ_TYPE, AT_ROOT);
    set_node_name(node, "root");

    return node;
}

/*
 * Allocate memory for a node with no data in it.
 */
ast_t* create_node(ast_t* parent) {

    ast_t* node = _alloc(sizeof(ast_t));
    node->parent = parent;
    node->name = NULL;
    node->left = NULL;
    node->right = NULL;
    node->attrs = NULL;
    node->children = NULL;

    set_node_scanner_attrs(node);
    if(parent != NULL) {
        set_node_attr_str(node, AT_PARENT_NAME, get_node_name(parent));
        set_node_attr_ptr(node, AT_PARENT_PTR, parent);
    }

    return node;
}

/*
 * Add the given node to the child tree instead of the root tree.
 */
void add_child_node(ast_t* parent, ast_t* node) {

    if(node->name == NULL)
        fatal("cannot add child tree node without a name");

    if(parent->children != NULL)
        add_node(parent->children, node);
    else
        parent->children = node;
}


/*
 * Add this node to the root tree. These are all at the same level and have the
 * same parent node.
 */
void add_node(ast_t* sibling, ast_t* node) {

    if(node->name == NULL)
        fatal("cannot add tree node without a name");

    node->parent = sibling->parent; // same parent as the sibling
    //set_node_scanner_attrs(node);   // add the file name, column and line number

    if(sibling != NULL) {
        int finished = 0;
        ast_t* tmp = sibling;

        while(!finished) {
            int res = strcmp(tmp->name, node->name);
            if(0 > res) {
                if(tmp->right == NULL) {
                    tmp->right = node;
                    finished++;
                }
                else
                    tmp = tmp->right;
            }
            else if(0 < res) {
                if(tmp->left == NULL) {
                    tmp->left = node;
                    finished++;
                }
                else
                    tmp = tmp->left;
            }
            else {
                // symbol exists. mark error and keep on parsing.
                syntax("symbol \"%s\" already exists.", node->name);
            }
        }
    }
    else
        fatal("invalid root tree node");
}

/*
 * Return the node with the name in the current level. Does not traverse
 * parents and children. Returns NULL if the node is not found.
 */
ast_t* find_node(ast_t* node, const char* name) {

    if(node != NULL) {
        int finished = 0;
        ast_t* tmp = node;

        while(!finished) {
            int res = strcmp(tmp->name, name);
            if(0 > res) {
                if(tmp->right != NULL)
                    tmp = tmp->right;
                else
                    return NULL;
            }
            else if(0 < res) {
                if(tmp->left != NULL)
                    tmp = tmp->left;
                else
                    return NULL;
            }
            else
                return tmp;
        }
    }

    return NULL;
}

void set_node_name(ast_t* node, const char* name) {

    node->name = _strdup(name);
    set_node_attr_str(node, AT_OBJ_NAME, name);
}

void set_node_attr_str(ast_t* node, attr_type_t type, const char* data) {

    size_t len = strlen(data) + 1;
    set_node_attr(node, type, (void*)data, len);
}

void set_node_attr_int(ast_t* node, attr_type_t type, int var) {

    // can do this because the data is copied.
    set_node_attr(node, type, (void*)(&var), sizeof(int));
}

// store the pointer, not the data that is pointed to.
void set_node_attr(ast_t* node, attr_type_t type, void* data, size_t size) {

    void* ptr;
    if(data != NULL) {
        ptr = _alloc(size);
        memcpy(ptr, data, size);
    }
    else
        ptr = NULL;

    set_node_attr_ptr(node, type, ptr);
}

void set_node_scanner_attrs(ast_t* node) {

    set_node_attr_str(node, AT_FILE_NAME, get_fname());
    set_node_attr_int(node, AT_LINE_NO, get_line_no());
    set_node_attr_int(node, AT_COL_NO, get_col_no());
}

void set_node_attr_ptr(ast_t* node, attr_type_t type, void* data) {

    if(node == NULL)
        return;

    attrib_t* nat = _alloc(sizeof(attrib_t));
    nat->type = type;
    nat->left = NULL;
    nat->right = NULL;
    nat->data = data;

    if(node->attrs != NULL) {
        attrib_t* at = node->attrs;
        int finished = 0;

        while(!finished) {
            if(at->type > type) {
                if(at->right == NULL) {
                    at->right = nat;
                    finished++;
                }
                else
                    at = at->right;
            }
            else if(at->type < type) {
                if(at->left == NULL) {
                    at->left = nat;
                    finished++;
                }
                else
                    at = at->left;
            }
            else {
                // replace the data, which could be NULL
                if(at->data != NULL)
                    _free(at->data);
                at->data = nat->data;
                _free(nat);
                finished++;
            }
        }
    }
    else
        node->attrs = nat;
}

// returns true if attribute is found, else return false.
bool get_node_attr(ast_t* node, attr_type_t type, void** data) {

    if(node == NULL)
        return false;

    if(node->attrs != NULL) {
        int finished = 0;
        attrib_t* tmp = node->attrs;

        while(!finished) {

            if(tmp->type > type) {
                if(tmp->right != NULL)
                    tmp = tmp->right;
                else
                    return false;
            }
            else if(tmp->type < type) {
                if(tmp->left != NULL)
                    tmp = tmp->left;
                else
                    return false;
            }
            else {
                *data = tmp->data;
                return true;
            }
        }
    }

    return false;
}

const char* get_node_name(ast_t* node) {

    if(node != NULL)
        return node->name;
    else
        return NULL;
}

/*
 * The return value is the noce that has the name specified, or NULL if it's
 * not found.
 */
ast_t* validate_name(ast_t* node, const char* name) {

// TODO: Implement this
    (void)name;
    return node;
}

attr_type_t tok_to_node_type(token_type t) {

    return (t == FLOAT_TOK)? AT_FLOAT_TYPE:
            (t == INT_TOK)? AT_INT_TYPE:
            (t == UINT_TOK)? AT_UINT_TYPE:
            (t == BOOL_TOK)? AT_BOOL_TYPE:
            (t == DICT_TOK)? AT_DICT_TYPE:
            (t == LIST_TOK)? AT_LIST_TYPE:
            (t == STRING_TOK)? AT_STRING_TYPE:
            (t == ANY_TOK)? AT_ANY_TYPE: AT_ERROR;
}


/*
 * Below this point the symbol table debug functions are implemented.
 */
#define DEPTH 4
static void recurse_dump(ast_t* node, int indent);   // forward reference

static void recurse_attrs(attrib_t* attr, int indent) {

    if(attr != NULL) {
        if(attr->left != NULL)
            recurse_attrs(attr->left, indent);

        if(attr->right != NULL)
            recurse_attrs(attr->right, indent);

        fprintf(stdout, "    %*s\"%s\" = ", indent, "", attr_type_to_str(attr->type));
        if(attr->data != NULL) {
            switch(attr->type) {
                // string attributes
                case AT_OBJ_NAME:
                case AT_FILE_NAME:
                case AT_PARENT_NAME:
                case AT_INHERIT_NAME:
                case AT_USER_TYPE_NAME:
                    fprintf(stdout, "%s\n",(const char*)attr->data);
                    break;

                // int attributes
                case AT_LINE_NO:
                case AT_COL_NO:
                    fprintf(stdout, "%d\n", *((int*)attr->data));
                    break;

                // type attributes
                case AT_OBJ_TYPE:
                case AT_RETURN_TYPE:
                case AT_SCOPE:
                    fprintf(stdout, "%s\n", attr_type_to_str(*(attr_type_t*)attr->data));
                    break;

                // pointer attributes
                case AT_PARENT_PTR:
                case AT_INHERIT_PTR:
                case AT_USER_TYPE_PTR:
                case AT_USER_DATA_PTR:
                case AT_OBJ_PTR:
                    fprintf(stdout, "%p\n", ((void*)attr->data));
                    break;

                case AT_CHILD_PTR:   // recurse into the tree
                    fprintf(stdout, "%p\n", ((void*)attr->data));
                    recurse_dump(attr->data, indent+DEPTH);
                    break;

                default:
                    fprintf(stdout, "error: unknown type\n");
                    break;
            }
        }
        else
            fprintf(stdout, "NULL\n");
    }
    else
        fprintf(stdout, "  no attributes\n");
}

static void recurse_dump(ast_t* node, int indent) {

    if(node->left != NULL)
        recurse_dump(node->left, indent);

    if(node->right != NULL)
        recurse_dump(node->right, indent);

    fprintf(stdout, "\n%*sName: %s (%p)\n", indent, "", node->name, node);
    recurse_attrs(node->attrs, indent);

    if(node->children != NULL)
        recurse_dump(node->children, indent+DEPTH);
}

void dump_tree(ast_t* root) {

    ENTER;
    fprintf(stdout, "-------------------------------\n");
    if(root != NULL) {
        recurse_dump(root, 0);
    }
    else {
        fprintf(stdout, "there are no symbols defined\n");
    }
    fprintf(stdout, "-------------------------------\n");
    LEAVE;
}


char const* attr_type_to_str(attr_type_t type) {

    return (type == AT_ERROR)? "ERROR":
        (type == AT_OBJ_NAME)? "OBJ_NAME":
        (type == AT_OBJ_PTR)? "OBJ_PTR":
        (type == AT_FILE_NAME)? "FILE_NAME":
        (type == AT_LINE_NO)? "LINE_NO":
        (type == AT_COL_NO)? "COL_NO":
        (type == AT_PARENT_PTR)? "PARENT_PTR":
        (type == AT_PARENT_NAME)? "PARENT_NAME":
        (type == AT_INHERIT_PTR)? "INHERIT_PTR":
        (type == AT_INHERIT_NAME)? "INHERIT_NAME":
        (type == AT_SCOPE)? "SCOPE":
        (type == AT_PUBLIC)? "PUBLIC":
        (type == AT_PRIVATE)? "PRIVATE":
        (type == AT_PROTECTED)? "PROTECTED":
        (type == AT_RETURN_TYPE)? "RETURN_TYPE":
        (type == AT_FLOAT_TYPE)? "FLOAT_TYPE":
        (type == AT_INT_TYPE)? "INT_TYPE":
        (type == AT_UINT_TYPE)? "UINT_TYPE":
        (type == AT_BOOL_TYPE)? "BOOL_TYPE":
        (type == AT_DICT_TYPE)? "DICT_TYPE":
        (type == AT_LIST_TYPE)? "LIST_TYPE":
        (type == AT_STRING_TYPE)? "STRING_TYPE":
        (type == AT_ANY_TYPE)? "ANY_TYPE":
        (type == AT_USER_TYPE)? "USER_TYPE":
        (type == AT_USER_TYPE_PTR)? "USER_TYPE_PTR":
        (type == AT_USER_DATA_PTR)? "USER_DATA_PTR":
        (type == AT_USER_TYPE_NAME)? "USER_TYPE_NAME":
        (type == AT_OBJ_TYPE)? "OBJ_TYPE":
        (type == AT_CLASS)? "CLASS":
        (type == AT_DATA_DECL)? "DATA_DECL":
        (type == AT_FUNC_DECL)? "FUNC_DECL":
        (type == AT_CHILD_PTR)? "CHILD_PTR":
        (type == AT_ROOT)? "ROOT NODE":
        "UNKNOWN";
}
