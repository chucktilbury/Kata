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


typedef struct _sattr {
    attr_type_t type;
    void* data;
    struct _sattr* left;
    struct _sattr* right;
} _sattr;

typedef struct _symbol {
    const char* name;
    _sattr* attrs;
    struct _symbol* left;
    struct _symbol* right;
} _symbol;

static _symbol* sym_table = NULL;

symbol create_symbol() {

    _symbol* sym = _alloc(sizeof(_symbol));
    sym->name = NULL;
    sym->left = NULL;
    sym->right = NULL;
    sym->attrs = NULL;

    return sym;
}

/*
 * Add the given raw symbol to the table with no attributes. If the symbol
 * already exists, then return NULL. Else return a pointer to the symbol.
 */
void add_symbol(symbol s, const char* name) {

    _symbol* sym = (_symbol*)s;
    sym->name = _strdup(name);

    // add the column and line number
    int val = get_line_no();
    set_symbol_attr(s, AT_LINE_NO, &val, sizeof(int));
    val = get_col_no();
    set_symbol_attr(s, AT_COL_NO, &val, sizeof(int));

    if(sym_table != NULL) {
        int finished = 0;
        _symbol* tmp = sym_table;

        while(!finished) {
            int res = strcmp(tmp->name, name);
            if(0 > res) {
                if(tmp->right == NULL) {
                    tmp->right = sym;
                    finished++;
                }
                else
                    tmp = tmp->right;
            }
            else if(0 < res) {
                if(tmp->left == NULL) {
                    tmp->left = sym;
                    finished++;
                }
                else
                    tmp = tmp->left;
            }
            else {
                // symbol exists. mark error and keep on parsing.
                syntax("symbol \"%s\" already exists.", name);
            }
        }
    }
    else
        sym_table = sym;
}

/*
 * Return an opaque handle of the symbol given with the name. If the symbol
 * does not exist, then return NULL.
 */
symbol get_symbol(const char* name) {

    if(sym_table != NULL) {
        int finished = 0;
        _symbol* tmp = sym_table;

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
                return (symbol)tmp;
        }
    }

    return NULL;
}

void set_symbol_attr_str(symbol sym, attr_type_t type, const char* data) {

    size_t len = strlen(data) + 1;
    set_symbol_attr(sym, type, (void*)data, len);
}

void set_symbol_attr_int(symbol sym, attr_type_t type, int var) {

    // can do this because the data is copied.
    set_symbol_attr(sym, type, (void*)(&var), sizeof(int));
}

// store the pointer, not the data that is pointed to.
void set_symbol_attr_ptr(symbol sym, attr_type_t type, void* var) {

    set_symbol_attr(sym, type, var, sizeof(void*));
}

void set_symbol_scanner_attrs(symbol sym) {

    int var;

    set_symbol_attr_str(sym, AT_FILE_NAME, get_fname());
    var = get_line_no();
    set_symbol_attr(sym, AT_LINE_NO, (void*)&var, sizeof(int));
    var = get_col_no();
    set_symbol_attr(sym, AT_COL_NO, (void*)&var, sizeof(int));
}

void set_symbol_attr(symbol sym, attr_type_t type, void* data, size_t size) {

    _symbol* s = (_symbol*)sym;
    if(s == NULL)
        return;

    _sattr* nat = _alloc(sizeof(_sattr));
    nat->type = type;
    nat->left = NULL;
    nat->right = NULL;
    if(data != NULL) {
        nat->data = _alloc(size);
        memcpy(nat->data, data, size);
    }
    else
        nat->data = NULL;

    if(s->attrs != NULL) {
        _sattr* at = s->attrs;
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
        s->attrs = nat;
}

// returns true if attribute is found, else return false.
int get_symbol_attr(symbol sym, attr_type_t type, void** data) {

    _symbol* s = (_symbol*)sym;
    if(s == NULL)
        return 0;

    if(s->attrs != NULL) {
        int finished = 0;
        _sattr* tmp = s->attrs;

        while(!finished) {

            if(tmp->type > type) {
                if(tmp->right != NULL)
                    tmp = tmp->right;
                else
                    return 0;
            }
            else if(tmp->type < type) {
                if(tmp->left != NULL)
                    tmp = tmp->left;
                else
                    return 0;
            }
            else {
                *data = tmp->data;
                return 1;
            }
        }
    }

    return 0;
}

const char* get_sym_name(symbol sym) {

    _symbol* s = (_symbol*)sym;
    return s->name;
}

attr_type_t tok_to_symbol_type(token_type t) {

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
static void recurse_attrs(_sattr* attr) {

    if(attr != NULL) {
        if(attr->left != NULL)
            recurse_attrs(attr->left);

        if(attr->right != NULL)
            recurse_attrs(attr->right);

        fprintf(stdout, "    \"%s\" = ", attr_type_to_str(attr->type));
        if(attr->data != NULL) {
            switch(attr->type) {
                case AT_NAME:
                case AT_SYMBOL_NAME:
                case AT_INHERIT_NAME:
                case AT_FILE_NAME:
                case AT_PARENT_NAME:
                    fprintf(stdout, "%s\n", (const char*)attr->data);
                    break;

                case AT_LINE_NO:
                case AT_COL_NO:
                    fprintf(stdout, "%d\n", *((int*)attr->data));
                    break;

                // this looks messy, but beware!
                case AT_REF_TYPE:
                case AT_USER_DEFINED:
                case AT_NATIVE_TYPE:
                case AT_SCOPE:
                case AT_OBJ_TYPE:
                case AT_PUBLIC:
                case AT_PRIVATE:
                case AT_PROTECTED:
                case AT_REF_TYPE_OF:
                    fprintf(stdout, "%s\n", attr_type_to_str(*(attr_type_t*)attr->data));
                    break;

                case AT_INHERIT_PTR:
                    fprintf(stdout, "%p\n", (void*)attr->data);
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
        fprintf(stdout, "    no attributes\n");
}

static void recurse_dump(_symbol* sym) {

    if(sym->left != NULL)
        recurse_dump(sym->left);

    if(sym->right != NULL)
        recurse_dump(sym->right);

    fprintf(stdout, "Name: %s\n", sym->name);
    recurse_attrs(sym->attrs);
}

void dump_symbols() {

    ENTER;
    if(sym_table != NULL) {
        recurse_dump(sym_table);
    }
    else {
        fprintf(stdout, "there are no symbols defined\n");
    }
    LEAVE;
}

char const* attr_type_to_str(attr_type_t type) {

    return (type == AT_NAME)? "NAME":
            (type == AT_SYMBOL_NAME)? "SYMBOL_NAME":
            (type == AT_LINE_NO)? "LINE_NO":
            (type == AT_COL_NO)? "COL_NO":
            (type == AT_OBJ_TYPE)? "OBJ_TYPE":
            (type == AT_FILE_NAME)? "FILE_NAME":
            (type == AT_SCOPE)? "SCOPE":
            (type == AT_PUBLIC)? "PUBLIC":
            (type == AT_PRIVATE)? "PRIVATE":
            (type == AT_PROTECTED)? "PROTECTED":
            (type == AT_INHERIT_NAME)? "INHERIT_NAME":
            (type == AT_INHERIT_PTR)? "INHERIT_PTR":
            (type == AT_REF_TYPE)? "REF_TYPE":
            (type == AT_REF_TYPE_OF)? "REF_TYPE_OF":
            (type == AT_USER_DEFINED)? "USER_DEFINED":
            (type == AT_NATIVE_TYPE)? "NATIVE_TYPE":
            (type == AT_FUNC_DEF)? "FUNC_DEF":
            (type == AT_DATA_DECL)? "DATA_DECL":
            (type == AT_FUNC_DECL)? "FUNC_DECL":
            (type == AT_FLOAT_TYPE)? "FLOAT_TYPE":
            (type == AT_INT_TYPE)? "INT_TYPE":
            (type == AT_UINT_TYPE)? "UINT_TYPE":
            (type == AT_BOOL_TYPE)? "BOOL_TYPE":
            (type == AT_DICT_TYPE)? "DICT_TYPE":
            (type == AT_LIST_TYPE)? "LIST_TYPE":
            (type == AT_STRING_TYPE)? "STRING_TYPE":
            (type == AT_ANY_TYPE)? "ANY_TYPE":
            (type == AT_OBJECT_TYPE)? "OBJECT_TYPE":
            (type == AT_CLASS_DECL)? "CLASS_DECL" : "unknown";
}
