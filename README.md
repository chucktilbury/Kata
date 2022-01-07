# Simple
The Simple programming language.

This is a very simple object-oriented programming language which is written in C++. There is no expectation that this will every be used by anyone. It's just a fun hobby project. 

The language is a synthesis or Python and C. It has the object model of Python but shares a lot of syntax with both C and C++.

General features in no particular order.
* Curly braces to delimit blocks. Semicolons are not required.
* Simplified type system, but is strongly typed with casting.
* Function overloads by parameter type.
* Void type is used.
* Classes with single inheritance only.
* No struct keyword.
* Supports dictionaries and lists natively.
* String is a data type.
* Managed memory.
* Exceptions.
* Support for external libraries.

Features that are specifically not supported.

* Pointers.
* Multiple inheritance.
* Monkey patching.
* Code that is not declared as part of a class.

This single-pass compiler accepts Simple source code and outputs C source code. The output should be compilable by any modern ANSI C compiler. The output is not really intended to be human readable, but the #line directives are used to aid debugging. 
