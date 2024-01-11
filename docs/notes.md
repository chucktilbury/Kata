# RD Implementation
All parsers are nothing more than a state machine. A parser generator creates a unique state machine for the specified grammar.
* Every RD function has a 2 dim array defined.
* The rows are the rule lines and the columns are the items in a line.
* The array contains function pointers.
* The arrays are defined in the functions as local vars.
* Functions in the array return a data structure of a particular type.
* If the parse that a function is intended to do fails, then it returns NULL.
  * When a function returns NULL, then the error handler is invoked.
  * The error handler accepts a pointer to the array and the current position. It generates the error based upon that information and does recovery.
  * Support for a custom recovery routine is required. If there is no routine defined, then simply exit the parser gracefully.
  * Do I want to place an "error" item in the AST data?
  * Does the parse function return after the error handler? (yes, I think so)

* When a rule ends it returns a "rule end" pointer so that the function knows to return its payload.
* All state transitions are handled by individual functions that are defined for every terminal and non-terminal that was defined for the grammar.

# AST building.
The AST is built by the state machine as a result of parsing. Traversing the AST is accomplished with a different set of library routines.
* Every AST node has pointers to other AST nodes.
  * These nodes represent both terminal and non-terminal symbols.
* When part of a rule is successfully parsed then an element is it's data structure is updated with a pointer to the the element that was parsed.

Functions return an AST node and that node is stored in the tree. Nodes all have a list of nodes, so the routines that traverse the tree can discover the nodes and perform actions based upon the result. The actions associated with a rule row are executed when the AST is traversed.

So maybe the rules do not need to be defined within a function if they contain function pointers. It is a reasonable assumption that any particular non-terminal will always be resolved in the same way, even if there are alternatives. Maybe a generic driver can be implemented that handles all aspects of AST generation based upon the tables.

```c
typedef void* (*array_func)();
```
* The void pointer returned is a pointer to an AST node that will be added to the current AST node list.
* If the function returns NULL then the error handler is called to print and "expected but got" style error. The error handler does recovery and also causes the current function to return. Maybe an error token should be put into the AST.
  * Note that a custom recovery routine is needed.
* There is a special return value that indicates the end of the rule. This is not added to the AST node. Instead, it causes the rule handler to return its payload.

There is a function defined for every single terminal and non-terminal that is defined in the grammar. That means that the arrays have to be defined in these functions and a generic driver is not possible. However, there is a lot of boilerplate that can be used. Even though this is wasteful from one perspective, it should work and is required.

## UPSHOT:
* Are the arrays defined in a rule-specific function? Or can there be a generic driver handle it? If the AST data structure only has a list of objects, then a generic driver could be used.
  * Note that the individual functions are required to return the AST data structure, but a generic driver is also required to call the functions. (is that true?)
* Need to do some research on how to properly implement multi-dim arrays. Do I need support routines?


# Random thoughts
* Change some of the data structures to linked lists instead of arrays of voids.
* Make prefix for files and output symbols
* Remove aname and pname.
* There is no need to add code to the end of the parser. The code that traverses the AST is what needs to access any custom generic code and so custom code should be placed at the beginning of the traversal routines.
* Code that is specified for rule rows is run when the AST is traversed, not when the parser completes a rule. Do I need code that runs when a rule is completed? I think things like checking symbols and other semantics should be done with AST passes.
  * How much of that can be automated?
* Implement Graphiz DOT file output to represent the parser bubble transitions for troubleshooting.



----
# Data below here is invalidated.
----
... and kept for historical reasons ...

----

# Thinking about parser generators

A state machine can be constructed by identifying the state changes. A state change is where one token can follow another. This requires the driver to trace through every possible parse of the grammar. If there is a duplicate parse, then that is a parser conflict. Every possible point at which a parse makes a decision is a state. Conflicts are resolved by ordering the rules.

~~Stack based parser. States are kept on a stack instead of a state machine. Look at it like a stack based interpreter that translates the grammar into C code.~~

- First invent a way to implement a state machine from tables.
- The state machine must run arbitrary code at arbitrary states.
- The state machine only needs to use numbers to describe states.



# Thinking code

Emitted code to parse a single rule.

```C
/*
    Rules
    // Simple rule that just accepts simple patterns
    plaster
        : bacon {}        : eggs {}
        : thing {}

    // The recursive rule accepts bacon, eggs or thing as an element.
    plaster_lst
        : plaster {}
        : plaster COMMA plaster_lst {}

    Data structures
    PTRLST_HEADER(plaster_lst, plaster_t*)
    typedef struct _plaster_ plaster_t;
    struct _plaster_ {
        bacon_t* m_bacon;
        eggs_t* m_eggs;
        plaster_lst_t* m_plaster_lst;
    };
*/

bool plaster_pat1(plaster_t* ptr) {

// If there is only one element in the pattern, then this is redundant
// however, it does simplify code generation.
    if(NULL != (ptr->bacon = bacon())) {
        return true;
    }

    return false;
}

bool plaster_pat2(plaster_t* ptr) {

    if(NULL != (ptr->eggs = eggs())) {
        return true;
    }

    return false;
}

bool plaster_pat3(plaster_t* ptr) {

    if(NULL != (ptr->thing = thing())) {
        return true;
    }

    return false;
}

static plaster_t* plaster() {

    plaster_t* tmp = _alloc_obj(plaster_t);

    if(plaster_pat1(tmp)) {
        // run the pattern code
        return tmp;
    }
    else if(plaster_pat2(tmp)) {
        // run the pattern code
        return tmp;
    }
    else if(plaster_pat3(tmp)) {
        // run the pattern code
        return tmp;
    }
    else {
        syntax_error("expected a plaster but got a %s", to_name(tmp));
        return NULL;
    }
}

static bool plaster_lst_pat1(plaster_lst_t* ptr) {

    plaster_t* tmp;
    if(NULL != (tmp = plaster())) {
        plaster_lst_add(ptr, tmp);
        consume_token();
        if(token_type == COLON)
            plaster_lst();
        else
        return true;
    }
}

static bool plaster_lst_pat2(plaster_lst_t* ptr) {

}

static plaster_lst_t* plaster_lst() {

    plaster_lst_t* ptr = _alloc_obj(plaster_lst_t);

    if(plaster_lst_pat1(ptr)) {
        // pattern code
        return ptr;
    }
    else if(plaster_lst_pat2(ptr)) {
        // pattern code
        return ptr;
    }
    else {
        syntax_error("expected but got ...");
        return NULL
    }
}

```