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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "symbols.h"
#include "errors.h"

typedef struct _sattr {
    const char* name;
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

/*
 * Add the given raw symbol to the table with no attributes. If the symbol
 * already exists, then return NULL. Else return a pointer to the symbol.
 */
symbol create_symbol(const char* name) {

    _symbol* sym = _alloc(sizeof(_symbol));
    sym->name = _strdup(name);
    sym->left = NULL;
    sym->right = NULL;
    sym->attrs = NULL;

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
                // symbol exists.
                syntax("symbol \"%s\" already exists.", name);
                _free((void*)sym->name);
                _free(sym);
                return NULL;
            }
        }
    }
    else
        sym_table = sym;

    return (symbol)sym;
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

void set_symbol_attr(symbol sym, const char* name, void* attr, size_t size) {

    _symbol* s = (_symbol*)sym;
    if(s == NULL)
        return;

    _sattr* nat = _alloc(sizeof(_sattr));
    nat->name = _strdup(name);
    nat->left = NULL;
    nat->right = NULL;
    nat->data = _alloc(size);
    memcpy(nat->data, attr, size);

    if(s->attrs != NULL) {
        _sattr* at = s->attrs;
        int finished = 0;

        while(!finished) {
            int res = strcmp(s->name, name);
            if(0 > res) {
                if(at->right == NULL) {
                    at->right = nat;
                    finished++;
                }
                else
                    at = at->right;
            }
            else if(0 < res) {
                if(at->left == NULL) {
                    at->left = nat;
                    finished++;
                }
                else
                    at = at->left;
            }
            else {
                // replace the data
                _free(at->data);
                at->data = nat->data;
                _free((void*)nat->name);
                _free(nat);
                finished++;
            }
        }
    }
    else
        s->attrs = nat;
}

void* get_symbol_attr(symbol sym, const char* name) {

    _symbol* s = (_symbol*)sym;
    if(s == NULL)
        return NULL;

    if(s->attrs != NULL) {
        int finished = 0;
        _sattr* tmp = s->attrs;

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
                return tmp->data;
        }
    }

    return NULL;
}
