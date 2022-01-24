/*
 * A class in Simple is a declaration, not a definition. The only things that
 * are allowed in a class declaration are data and function declarations. No
 * initializations are allowed. Therefore the data_decl() and func_decl() are
 * static function defined here. The class is emitted as it is read. The
 * scope keywords are active as well, which are public, private and protected.
 */
#include "common.h"

/*
 * When this is entered, the current token is the OPAREN_TOK. When it exits,
 * the current token will be the one past the CPAREN_TOK.
 *
 * A parameter list declares local vars to a function. It consists of type/symbol
 * pairs that are separated by commas.
 */
static void parse_parameter_list(ast_t* parent) {

    token* tok = next_token(); // eat the '(', which has already been validated.
    bool finished = false;
    int state = 0;
    ast_t* node;

    while(!finished) {
        switch(state) {
            case 0:
                node = create_node(parent);
                set_node_attr_int(node, AT_OBJ_TYPE, AT_DATA_DECL);
                state = 1;
                break;

            case 1:
                switch(tok->type) {
                    case FLOAT_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_FLOAT_TYPE);
                        set_node_attr(node, AT_USER_DATA_PTR, &tok->data.fnum, sizeof(double));
                        break;

                    case INT_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_INT_TYPE);
                        set_node_attr(node, AT_USER_DATA_PTR, &tok->data.inum, sizeof(int64_t));
                        break;

                    case UINT_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_UINT_TYPE);
                        set_node_attr(node, AT_USER_DATA_PTR, &tok->data.unum, sizeof(uint64_t));
                        break;

                    case BOOL_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_BOOL_TYPE);
                        set_node_attr(node, AT_USER_DATA_PTR, &tok->data.boolean, sizeof(bool));
                        break;

                    case DICT_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_BOOL_TYPE);
                        set_node_attr_ptr(node, AT_USER_DATA_PTR, tok->data.obj);
                        break;

                    case LIST_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_LIST_TYPE);
                        set_node_attr_ptr(node, AT_USER_DATA_PTR, tok->data.obj);
                        break;

                    case STRING_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_STRING_TYPE);
                        set_node_attr_ptr(node, AT_USER_DATA_PTR, tok->data.obj);
                        break;

                    case ANY_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_ANY_TYPE);
                        break;

                    case SYMBOL_TOK: // user defined type from local class
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_USER_TYPE);
                        set_node_attr_ptr(node, AT_USER_TYPE_PTR, validate_name(parent, tok->str));
                        set_node_attr_str(node, AT_USER_TYPE_NAME, tok->str);
                        break;

                    case COMPOUND_NAME_TOK: // user defined type from an import
                        // verify that the type exists and is compatible.
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_USER_TYPE);
                        set_node_attr_ptr(node, AT_USER_TYPE_PTR, validate_name(parent, tok->str));
                        set_node_attr_ptr(node, AT_USER_DATA_PTR, tok->data.obj);
                        set_node_attr_str(node, AT_USER_TYPE_NAME, tok->str);
                        break;

                    case CPAREN_TOK:  // ending the loop
                        finished = true;
                        next_token();
                        break;

                    default:
                        syntax("expected a scope operator or a type but got %s", tok_to_str(tok->type));
                        finished = true;
                        break;
                }
                state = 2;
                next_token();
                break;

            case 2:
                switch(tok->type) {
                    case SYMBOL_TOK: // user defined type from local class
                        set_node_name(node, tok->str);
                        add_child_node(parent, node);
                        next_token();
                        state = 3;
                        break;

                    case COMPOUND_NAME_TOK: // user defined type from an import
                        syntax("cannot use a compound name in a parameter list");
                        finished = true;
                        break;

                    default:
                        syntax("expected a name but got %s", tok_to_str(tok->type));
                        finished = true;
                        break;
                }
                break;

            case 3:
                if(tok->type == CPAREN_TOK) {
                    state = 4;
                    next_token();
                }
                else if(tok->type == COMMA_TOK) {
                    state = 0;
                    next_token();
                }
                else {
                    syntax("expected a close paren or a comma but got %s", tok_to_str(tok->type));
                    finished = true;
                }

                break;

            case 4:
                finished = true;
                break;
        }
    }
}

/*
 * When this is entered, the current token is the one that is past the
 * OCURLY_TOK. When it exits, the current token is the one past the CCURLY_TOK.
 */
static void parse_class_lines(ast_t* parent) {

    token* tok = get_token();   // returns the pointer to the current token.
    bool finished = false;
    int state = 0;
    ast_t* node;
    attr_type_t scope = AT_PRIVATE;

    while(!finished) {
        switch(state) {
            case 0: // create a new node
                node = create_node(parent);
                state = 1;
                break;

            case 1: // starting a new line
                switch(tok->type) {
                    case PUBLIC_TOK:
                        //set_node_attr_int(node, AT_SCOPE, AT_PUBLIC);
                        scope = AT_PUBLIC;
                        state = 3;
                        next_token();
                        break;

                    case PRIVATE_TOK:
                        //set_node_attr_int(node, AT_SCOPE, AT_PRIVATE);
                        scope = AT_PRIVATE;
                        state = 3;
                        next_token();
                        break;

                    case PROTECTED_TOK:
                        //set_node_attr_int(node, AT_SCOPE, AT_PROTECTED);
                        scope = AT_PROTECTED;
                        state = 3;
                        next_token();
                        break;

                    case CCURLY_TOK: // finish up
                        state = 2;
                        break;

                    default:
                        state = 3;
                        break;
                }
                break;

            case 2: // last line (a }) was parsed
                finished = true;
                //next_token(); // eat the CCURLY_TOK
                break;

            case 3: // next thing MUST be a type
                set_node_attr_int(node, AT_SCOPE, scope);
                switch(tok->type) {
                    case FLOAT_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_FLOAT_TYPE);
                        set_node_attr(node, AT_USER_DATA_PTR, &tok->data.fnum, sizeof(double));
                        break;

                    case INT_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_INT_TYPE);
                        set_node_attr(node, AT_USER_DATA_PTR, &tok->data.inum, sizeof(int64_t));
                        break;

                    case UINT_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_UINT_TYPE);
                        set_node_attr(node, AT_USER_DATA_PTR, &tok->data.unum, sizeof(uint64_t));
                        break;

                    case BOOL_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_BOOL_TYPE);
                        set_node_attr(node, AT_USER_DATA_PTR, &tok->data.boolean, sizeof(bool));
                        break;

                    case DICT_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_BOOL_TYPE);
                        set_node_attr_ptr(node, AT_USER_DATA_PTR, tok->data.obj);
                        break;

                    case LIST_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_LIST_TYPE);
                        set_node_attr_ptr(node, AT_USER_DATA_PTR, tok->data.obj);
                        break;

                    case STRING_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_STRING_TYPE);
                        set_node_attr_ptr(node, AT_USER_DATA_PTR, tok->data.obj);
                        break;

                    case ANY_TOK:
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_ANY_TYPE);
                        break;

                    case SYMBOL_TOK: // user defined type from local class
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_USER_TYPE);
                        set_node_attr_ptr(node, AT_USER_TYPE_PTR, validate_name(parent, tok->str));
                        set_node_attr_str(node, AT_USER_TYPE_NAME, tok->str);
                        break;

                    case COMPOUND_NAME_TOK: // user defined type from an import
                        // verify that the type exists and is compatible.
                        set_node_attr_int(node, AT_RETURN_TYPE, AT_USER_TYPE);
                        set_node_attr_ptr(node, AT_USER_TYPE_PTR, validate_name(parent, tok->str));
                        set_node_attr_ptr(node, AT_USER_DATA_PTR, tok->data.obj);
                        set_node_attr_str(node, AT_USER_TYPE_NAME, tok->str);
                        break;

                    default:
                        syntax("expected a scope operator or a type but got %s", tok_to_str(tok->type));
                        finished = true;
                        break;
                }
                next_token();
                state = 4;  // next thing must be a name
                break;

            case 4: // expecting a simple name after the type specifier
                switch(tok->type) {
                    case SYMBOL_TOK:
                        set_node_name(node, tok->str);
                        add_child_node(parent, node);
                        next_token();
                        state = 5;
                        break;

                    case COMPOUND_NAME_TOK:
                        syntax("flarp!");
                        finished = true;
                        break;

                    default:
                        syntax("flibble!");
                        finished = true;
                        break;
                }
                break;

            case 5: // check for a parameter list
                switch(tok->type) {
                    case OPAREN_TOK:
                        set_node_attr_int(node, AT_OBJ_TYPE, AT_FUNC_DECL);
                        parse_parameter_list(node);
                        state = 0;  // get the next item in the class declaration.
                        break;

                    default:
                        set_node_attr_int(node, AT_OBJ_TYPE, AT_DATA_DECL);
                        state = 0;  //
                        break;
                }
                break;
        }
    }
}

/*
 * When this is entered, a scope operator or the class keyword have been read.
 * When it exits, the token past the closing CCURLY_TOK is the current one.
 */
void parse_class(ast_t* parent) {

    token* tok = get_token();   // returns the pointer to the current token.
    bool finished = false;
    int state = 0;
    ast_t* node = create_node(parent);
    set_node_attr_int(node, AT_OBJ_TYPE, AT_CLASS);

    while(!finished) {
        switch(state) {
            case 0:
                switch(tok->type) {
                    case PUBLIC_TOK:
                        set_node_attr_int(node, AT_SCOPE, AT_PUBLIC);
                        next_token();
                        state = 2;
                        break;

                    case PRIVATE_TOK:
                        set_node_attr_int(node, AT_SCOPE, AT_PRIVATE);
                        next_token();
                        state = 2;
                        break;

                    case PROTECTED_TOK:
                        syntax("a class cannot have protected scope");
                        finished = true;
                        break;

                    case CLASS_TOK:
                        state = 2;  // parse the symbol into the node
                        break;

                    default:
                        syntax("expected a scope operator or the class keyword but got %s", tok_to_str(tok->type));
                        finished = true;
                        break;
                }
                break;

            case 1: // state machine is finished
                if(tok->type != CCURLY_TOK)
                    syntax("expected a %s but got %s", tok_to_str(CCURLY_TOK), tok_to_str(tok->type));
                finished = true;
                next_token();
                break;

            case 2: // have a scope token, next must be the class token.
                if(tok->type != CLASS_TOK) {
                    syntax("expected class keyword but got %s", tok_to_str(tok->type));
                    finished = true;
                }
                else {
                    next_token();
                    state = 3;
                }
                break;

            case 3: // parse the symbol into the node
                switch(tok->type) {
                    case SYMBOL_TOK:
                        set_node_name(node, tok->str);
                        add_child_node(parent, node);
                        next_token();
                        state = 4;
                        break;

                    case COMPOUND_NAME_TOK:
                        syntax("a compound name cannot be used to name a class");
                        finished = true;
                        break;

                    default:
                        syntax("expected a symbol but got %s", tok_to_str(tok->type));
                        finished = true;
                        break;

                }
                break;

            case 4:
                if(tok->type == OCURLY_TOK) {
                    next_token();
                    parse_class_lines(node);
                    state = 1;
                }
                else {
                    syntax("expected a %s but got %s", tok_to_str(OCURLY_TOK), tok_to_str(tok->type));
                    finished = true;
                }
                break;
        }
    }
}

#if 0
/*
 * Node parameter is the class that will be a parent to these elements.
 */
void parse_class_body(ast_t* node) {

    validate_token(get_token(), 1, OCURLY_TOK);
    validate_token(next_token(), 1, CCURLY_TOK);

    //class_params(node);
    next_token();   // class
    (void)node;
}

/*
 * Get the class definition. When this is entered, a public, private, or class
 * token has been read and will be the current token in the stream.
 */
void parse_class(ast_t* root) {


    token* tok = get_token();
    bool finished = false, flag = false;
    attr_type_t scope = AT_PRIVATE;
    ast_t* node = create_node(root, AT_CLASS);
    set_node_attr_int(node, AT_OBJ_TYPE, AT_CLASS);
    set_node_attr_str(node, AT_PARENT_NAME, get_node_name(root));
    set_node_attr_ptr(node, AT_PARENT_PTR, root);

    while(!finished) {
        switch(tok->type) {
            case PUBLIC_TOK:
                if(!flag) {
                    scope = AT_PUBLIC;
                    flag = true;
                    next_token();
                }
                else {
                    syntax("more than one scope operator is not allowed");
                    finished = true;
                }
                break;

            case PRIVATE_TOK:
                if(!flag) {
                    scope = AT_PRIVATE;
                    flag = true;
                    next_token();
                }
                else {
                    syntax("more than one scope operator is not allowed");
                    finished = true;
                }
                break;

            case PROTECTED_TOK:
                syntax("a class cannot have protected scope");
                finished = true;
                break;

            case CLASS_TOK:
                set_node_attr_int(node, AT_SCOPE, scope);
                // next token must be a name
                tok = next_token();
                switch(tok->type) {
                    case SYMBOL_TOK:
                        set_node_attr_str(node, AT_OBJ_NAME, tok->str);
                        set_node_name(node, tok->str);
                        set_node_scanner_attrs(node);
                        add_child_node(root, node);
                        next_token();
                        //parse_class_body(node);
                        break;
                    case COMPOUND_NAME_TOK:
                        syntax("a class definition name must be a simple name, not a compound name");
                        break;
                    default:
                        syntax("expected a name but got %s", tok_to_str(tok->type));
                        break;
                }
                finished = true;
                break;

            case OCURLY_TOK:
                parse_class_body(node);
                break;

            default:
                // should be caught in parse.c, but do it anyway.
                syntax("expected scope operator or class keyword, but got %s", tok_to_str(tok->type));
                finished = true;
                break;
        }
    }
}
#endif
