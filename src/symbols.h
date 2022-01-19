#ifndef SYMBOLS_H
#define SYMBOLS_H

typedef enum {
    AT_ERROR,       // errors...

    AT_SYMBOL_NAME, // decorated lookup name
    AT_NAME,        // base name
    AT_INHERIT_NAME,// class is a child of this
    AT_INHERIT_PTR, // pointer to the symbol of the parent

    AT_OBJ_TYPE,
    AT_FUNC_DEF,
    AT_CLASS_DECL,
    AT_DATA_DECL,
    AT_FUNC_DECL,

    AT_LINE_NO,     // scanner info
    AT_COL_NO,
    AT_FILE_NAME,

    AT_SCOPE,       // name of the attribute
    AT_PUBLIC,      // values for this name
    AT_PRIVATE,
    AT_PROTECTED,

    AT_REF_TYPE_OF, // name of attribute
    AT_USER_DEFINED,// values
    AT_NATIVE_TYPE,
    AT_PARENT_NAME,

    AT_REF_PTR,     // pointer to a reference definition in the symbol table.

    AT_REF_TYPE,    // name of attribute
    AT_FLOAT_TYPE,       // values
    AT_INT_TYPE,
    AT_UINT_TYPE,
    AT_BOOL_TYPE,
    AT_DICT_TYPE,
    AT_LIST_TYPE,
    AT_STRING_TYPE,
    AT_ANY_TYPE,
    AT_OBJECT_TYPE,

} attr_type_t;


typedef void* symbol;

symbol create_symbol();
void add_symbol(symbol sym, const char* name);
symbol get_symbol(const char* name);
void set_symbol_attr(symbol sym, attr_type_t type, void* attr, size_t size);
void set_symbol_attr_str(symbol sym, attr_type_t type, const char* attr);
void set_symbol_attr_int(symbol sym, attr_type_t type, int var);
void set_symbol_attr_ptr(symbol sym, attr_type_t type, void* var);
void set_symbol_scanner_attrs(symbol sym);
int get_symbol_attr(symbol sym, attr_type_t type, void** data);
const char* get_sym_name(symbol sym);
attr_type_t tok_to_symbol_type(token_type t);

void dump_symbols();
char const* attr_type_to_str(attr_type_t type);

#endif
