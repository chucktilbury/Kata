# Simple Parser
This is a hand-written recursive decent parser. It outputs an Abstract Semantic Tree. This document is mostly about the format of the AST. Note that I did not call it an abstract syntax tree. That's because the tree captures the semantics of the input and not the syntax. The syntax that was represented by the input is thrown away because it's not needed to output the actual program.

An AST node is actually a pointer to a hash table, whos functionality is defined in the ``util`` submodule. This approach sacrifices memory efficiency and some speed for a more general use model. There are about 50 non-terminal symbols and about 40 terminals. Otherwise, a separate data structure would need to be defined for each one. Since this is a hand-written parser, simplifying the data structures is beneficial.

Since this parser deals only with tokens, non-terminals and terminals are all represented symbolically as an enum. These are defined in the ast.h parser file.

The hash table contains all of the attributes associated with the node. Some nodes have their attributes in a specific order and those have a list fata structure as one of the attributes. All nodes have at minimum a ``type`` attribute and at lease one attribute that represents the payload of the node. The type entry of the node is unique for the grammar that the parser implements. This applies to both terminals and non-terminals that are stored in the node. Not all terminals or non-terminal symbols are stored in the AST. For example, there is no need to store the ``CLASS`` terminal in the ``class_definition`` non-terminal.

Every AST node is named after the non-terminal that created the node. For example, the top-most entry that is returned by the parser (called ``module``) has a single entry called ``module_list``. When one of the tree evaluators runs, it knows that it should expect a ``module_list`` as the first thing that it encounters when it traverses the list. We know that a ``module_list`` is a list of non-terminals.

* The data structure in the ``module`` node is a hash table, are is the data structure in all nodes.
* The "type_name" entry returns a node type of "module" in a String. All nodes have a "type_name".
* The "type" entry returns a pointer to an enum called AST_MODULE. All nodes have a "type".
* The ``module`` handler function expects to see an entry called "module_list". The ``module_list`` entry will be a pointer to a PtrList which contains all of the non-terminals that were parsed for the module. This is a list because it needs to be evaluated in the order that it was defined in the input.

Another example is a expression element. Expressions are basically a tree where a binary operator is a node and the values are leafs. However, unary operators are leafs. So a binary expression has entries of ``rhs``, ``lhs``, and ``oper``, as well as ``type``. There also needs to be an entry to give the type of the operation. There are 3 possible types for an expression. They are ``STRING``, ``NUMERIC``, and ``BOOLEAN``. (in that order of precedence)