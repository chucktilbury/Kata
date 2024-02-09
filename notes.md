
# Problems...

* The parser needs a global status. This status will keep the current name context and other information that is related to the global parsing context.

* The AST is a hash table. The AST is scanned to implement a multi-pass compiler such that names do not need to be defined in advance of them being referenced. Things like verifying that names have been defined are implemented as iterations of scanning the AST. The hash table:
  * allows node attributes to be added and updated as required
  * allows any arbitrary data type to be added as a node attribute
  * only requires a single data type to implement the AST

* When the token queue is rewound, it should only step back as far as the current rule has been read. When alternatives are evaluated they need to start in the stream where the function was called. So it looks more like a stack than like a queue. This makes sense in view of how a LALR(n) operates. The difference here is that the stack is not evaluated directly, but in code so to speak.
  * When a terminal actually gets used and the AST node is returned, then mark it as used. Then at some point, check the token stream. If the one at the head is marked as used, then dispose of each one in order until one that has not been marked is found. Then make that the new head.

* The import keyword switches the input file and also creates a namespace with the name of the imported file. That means that the scanner needs to report when a file ends and the parser needs to parse for that to know when to end the namespace.
  * Change fileio.c so that instead of closing files automatically, it is closed manually. The parser then controls when the file is actually closed. New functions of push_input_file() and pop_input_file(). Instead of only returning a EOF token to represent the end of all input, return EOF as the end of a file and an EOI token to represent the end of the input. The end of input happens where the last file has been popped from the file stack.
  * The scanner needs to return the EOF and EOI as tokens so that the parser can see when to make a change to the current namespace.
  * <DONE> Added end of file and end of input tokens to the scanner and   verified that they are being properly returned.