# Status

# Current
* The parser and AST are mostly finished. Writing tests and fixing bugs as they are found.

## Next steps
* Implement symbol table in the parser with fully qualified and decorated names.
  * When a new symbol is defined in souce code, it is written to a separate symbol table.
* Implement more of the import functionality.
  * When a file is being imported, only the name definitions are read by the parser. 
  * Type information is saved in the symbol table for future reference.
* The AST will verify the existance of symbols and veryify type information.
  * The AST will verify symbols from the symbol table to make it faster.

## Future
* Benchmark the parser.
* Implement the code emitter.
* Implement the runtime.
* Implement libraries.
* Implement IDE and debugger.