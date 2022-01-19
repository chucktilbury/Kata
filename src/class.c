/*
 * A class in Simple is a declaration, not a definition. The only things that
 * are allowed in a class declaration are data and function declarations. No
 * initializations are allowed. Therefore the data_decl() and func_decl() are
 * static function defined here. The class is emitted as it is read. The
 * scope keywords are active as well, which are public, private and protected.
 */
#include "common.h"

/*
 * The current token is the type of the definition. The next element is the
 * name. If the following element is not a '(' then it's a data declaration,
 * else it's a function declaration.
 */
void parse_class_body_element2(symbol cls, scope_t scope) {

    token* tok = get_token();
    symbol sym = create_symbol();

    // get the required type of the item
    switch(tok->type) {
        // user defined type
        case SYMBOL_TOK:
        case COMPOUND_NAME_TOK:
            // user defined types are referenced by name
            set_symbol_attr_int(sym, AT_REF_TYPE_OF, AT_USER_DEFINED);
            set_symbol_attr_int(sym, AT_REF_TYPE, AT_OBJECT_TYPE);
            set_symbol_attr_int(sym, AT_REF_PTR, NULL);
            //const char* s = cat_strings(3, get_sym_name(cls), ".", tok->str);
            //set_symbol_attr_str(sym, AT_NAME, s);
            break;
        // native type
        case FLOAT_TOK:
        case INT_TOK:
        case UINT_TOK:
        case BOOL_TOK:
        case DICT_TOK:
        case LIST_TOK:
        case STRING_TOK:
        case ANY_TOK:
            // native types are referenced by token type
            set_symbol_attr_int(sym, AT_REF_TYPE_OF, AT_NATIVE_TYPE);
            set_symbol_attr_int(sym, AT_REF_TYPE, tok_to_symbol_type(tok->type));
            break;
        default:
            syntax("expected type but got %s", tok_to_str(tok->type));
            return;
    }

    // get the required name
    tok = next_token();
    if(validate_token(tok, 1, SYMBOL_TOK)) {
// TODO: add the value union of the symbol as well.
        set_symbol_attr_str(sym, AT_NAME, tok->str);
        const char* s = cat_strings(3, get_sym_name(cls), ".", tok->str);
        set_symbol_attr_str(sym, AT_SYMBOL_NAME, s);
        add_symbol(sym, s);
        _free(s);
// TODO: add the logic to read the parameter list.
// TODO: add the logic to attach the sub-symbol table to the symbol for the
//  parameter list.
        next_token();
    }
    else {
        syntax("expected a symbol name, but got %s", tok_to_str(tok->type));
        return;
    }

}

/*
 * When this is read, the OCURLY has been read. Names are actually defined here.
 * all other instances of names are references.
 */
void parse_class_body_element1(symbol sym) {

    token* tok = get_token();
    bool finished = false;
    scope_t scope = PRIV_SCOPE;

    do {
        switch(tok->type) {
            // simply change the current scope
            case PUBLIC_TOK:    scope = PUB_SCOPE;  next_token(); break;
            case PRIVATE_TOK:   scope = PRIV_SCOPE; next_token(); break;
            case PROTECTED_TOK: scope = PROT_SCOPE; next_token(); break;

            // this happens when the class definition is empty
            case CCURLY_TOK:
                finished = true;
                // current token is ccurly
                break;

            // user defined type
            case SYMBOL_TOK:
            case COMPOUND_NAME_TOK:
            // native type
            case FLOAT_TOK:
            case INT_TOK:
            case UINT_TOK:
            case BOOL_TOK:
            case DICT_TOK:
            case LIST_TOK:
            case STRING_TOK:
            case ANY_TOK:
                finished = true;
                parse_class_body_element2(sym, scope);
                break;

            default:
                syntax("expected scope operator or type but got %s", tok_to_str(tok->type));
                finished = true;

        }
    } while(!finished);
}

/*
 * Parse the class body elements.
 */
void parse_class_body(symbol sym) {

    token* tok = get_token();
    bool finished = false;

    validate_token(tok, 1, OCURLY_TOK);
    tok = next_token(); // first token of the definition

    do {
        parse_class_body_element1(sym);
    } while(tok->type != CCURLY_TOK);

    // current token is ccurly
    next_token();
}

/*
 * Parse the base class name, if there is one.
 *
 * When this is entered, the current token must be a '(' or a '{'.
 */
void parse_class_base(symbol sym) {

    token* tok = get_token();

    if(tok->type == OPAREN_TOK) {
        tok = next_token();
        if(tok->type == COMPOUND_NAME_TOK || tok->type == SYMBOL_TOK) {
            set_symbol_attr_str(sym, AT_INHERIT_NAME, tok->str);
            set_symbol_attr_ptr(sym, AT_INHERIT_PTR, sym);

            tok = next_token();
            validate_token(tok, 1, CPAREN_TOK);
            next_token();
            parse_class_body(sym);
        }
        else if(tok->type == CPAREN_TOK) {
            set_symbol_attr_ptr(sym, AT_INHERIT_PTR, NULL);
            next_token();
            parse_class_body(sym);
        }
        else {
            syntax("expected a compound name, or a simple name, or ')', but got %s", tok_to_str(tok->type));
        }
    }
    else if(tok->type == OCURLY_TOK) {
        parse_class_body(sym);
    }
    else {
        syntax("expected %s or %s but got %s",
                tok_to_str(OPAREN_TOK),
                tok_to_str(OCURLY_TOK),
                tok_to_str(tok->type));
    }
}


/*
 * Parse the inheritance spec of the class definition.
 *
 * When this is entered, the class name symbol is the current token.
 */
static void parse_class_next(scope_t scope) {

    token* tok = get_token();

    if(tok->type == SYMBOL_TOK) {
        symbol sym = create_symbol();
        set_symbol_attr_int(sym, AT_OBJ_TYPE, AT_CLASS_DECL);
        set_symbol_attr_str(sym, AT_NAME, tok->str);
        set_symbol_scanner_attrs(sym);
        add_symbol(sym, tok->str);
        switch(scope) {
            case PUB_SCOPE:
                set_symbol_attr_int(sym, AT_SCOPE, AT_PUBLIC);
                break;
            case PRIV_SCOPE:
                set_symbol_attr_int(sym, AT_SCOPE, AT_PRIVATE);
                break;
            case PROT_SCOPE:
                syntax("protected scope is not allowed for class definition");
                break;
        }
        next_token();
        parse_class_base(sym);
    }
    else {
        syntax("expected symbol but got %s", tok_to_str(tok->type));
    }
}

/*
 * Retrieve a class with the optional single inheritance. A class can have data
 * declarations and function declarations. The create() and destroy() ctor and
 * dtor definitions are optional. The user defined ctor and dtor are executed
 * after the default ones.
 *
 * When this is entered, the current token could be a scope operator or the
 * class keyword.
 *
 */
void parse_class() {

    bool finished = false;
    bool scope_flag = false;
    scope_t scope = PRIV_SCOPE;
    token* tok = get_token();

    while(!finished) {
        switch(tok->type) {
            case PUBLIC_TOK:
                if(!scope_flag) {
                    scope = PUB_SCOPE;
                    scope_flag = true;
                    next_token();
                }
                else {
                    syntax("expected class keyword but got %s", tok_to_str(tok->type));
                    info("only one scope operator is allowed");
                    finished = true;
                }
                break;
            case PRIVATE_TOK:
                if(!scope_flag) {
                    scope = PRIV_SCOPE;
                    scope_flag = true;
                    next_token();
                }
                else {
                    syntax("expected class keyword but got %s", tok_to_str(tok->type));
                    info("only one scope operator is allowed");
                    finished = true;
                }
                break;
            case CLASS_TOK:
                next_token();
                parse_class_next(scope);
                finished = true;
                break;
            default:
                syntax("expected scope operator or class keyword, but got %s", tok_to_str(tok->type));
                finished = true;
                break;
        }
    }
}
