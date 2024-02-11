# UTIL

* TODO:
* Integrate the exceptions into the library.
* Implement sorting and searching for lists.
* Implement using a negative index addresses the end of the list

This is a library of routines that I find myself using over and over. So, instead of copying them to a new project, I will use this library to capture updates and changes. There is no guarantee that this will remain the same going forward. All of the things in this library are very simple and subject to change.

Likely changes that will happen in the future are
* Expanded handling of strings
  * Insert into interior of string
  * Prepend string.
  * Remove test from interior of string.
  * Strip white from ends.
* Enhancements for lists
  * Sorting a list, based on a user-defined compare callback.
  * Searching based on user-defined callback.
  * Deleting from the interior of the list
  * Detect changes to list while iterating
* Enhancements to the cmd parser
  * Add ability to form lists for a single parameter using the ',' character.
  * Better handling for boolean toggles
* Enhancements to hash tables
  * Store void pointers instead of internal memory allocations.
* Enhancements to memory functions
  * Integrate the garbage collector into the tree using embedded GIT project.
* Implement exceptions using setjmp standard library and use them in this library.

## MEM

This is a memory allocation interface. Its main purpose is to put a wrapper around memory allocation to detect allocation errors and abort the program if it runs out of memory. This particular library is able to switch to using the Boehm-Demers-Weiser Garbage Collector, which can be had here: https://github.com/ivmai/bdwgc. If you want to use the garbage collection then install the library and build this library with the ``USE_GC`` define set and the interface to it will be used instead of the standard library. In general, these macros behave exactly as the standard library routines except that they catch their own errors.

### API

```C
//  Allocate a block of memory the size of **s**
_ALLOC(s)

//  Allocate a data structure of **t** type.
_ALLOC_T(t)

//  Allocate an array of **n** data structures ot type **t**
_ALLOC_ARRAY(t, n)

//  Re-allocate a pointer **p** to **s** size.
_REALLOC(p, s)

//  Reallocate a pointer **p** to be the size of data structure **s**
_REALLOC_T(p, t)

// Reallocate an arrat of data structures
_REALLOC_ARRAY(p, t, n)

// Duplicate a memory object of size **s**
_DUP_MEM(p, s)

// Duplicate a string **p**.
_DUP_STR(p)

// Free a block of memory. This is a no-op when using GC.
_FREE(p)
```

## PTRLST

This manages a variable length list of void pointers. It can be used directly or it can be coerced into other data types using macros. For example, to store an array of data structures, where the attay will be grown automatically. Provisions are made to operate the array as a stack and also to iterate the array in an easy and transparent manner.

### API

```C
// Create a new and empty pointer list data structure.
PtrList* create_ptr_list();

// Destroy all memory allocated while creating and adding to this data
// structure. However, memory that was not allocated by this API must
// be free()d by the API that allocated it. In the case of using the GC,
// this function is a no-op.
void destroy_ptr_list(PtrList* h);

// Append a new pointer to the end of the pointer list.
void add_ptr_list(PtrList* h, void* ptr);

// Make the internal book keeping of the pointer list set to the beginning of
// the list. This routine must be called before iterating the list when
// starting at the beginning of it. This has no effect on the length of the
// list or the placement of the items in it.
void reset_ptr_list(PtrList* h);

// Iterate the list starting at the current location of the internal index. For
// example, if you partially iterate the list and then stop, you must reset the
// list in order to access the beginning of the list with this function.
void* iterate_ptr_list(PtrList* h);

// Same as add_ptr_list(). Append the pointer to the end of the list.
void push_ptr_list(PtrList* h, void* ptr);

// Retrieve the pointer on the top of the stack, which is always the end of
// the list. If the list is empty, then return NULL.
void* peek_ptr_list(PtrList* h);

// Remove the last item in the list and return a pointer to the new last item
// in the list.
void* pop_ptr_list(PtrList* h);
```

## STR

### String Lists

String lists use the pointer lists to implement a list of pointers to Str data structure. For detailed information about the API, look at the PTRLST API. The only difference is that instead of void pointers, this API implements pointers to the Str data structure.

### Strings

The strings API is a very simple and basic way to handle variable length strings efficiently. This API exists to add to the strings and iterate them without a bunch of typing.

### API

```C
// Create an empty list of pointers to Str.
StrList* create_str_list();

// Destroy a list of pointers to Str. When using GC, this function is a no-op.
void destroy_str_list(StrList* lst);

// Concatinate the string data strucutre to the list of strings.
void add_str_list(StrList* lst, Str* str);

// Reset the list of strings prior to iteration.
void reset_str_list(StrList* lst);

// Return the strings in the list one at a time with each call to this
// funciton. Returns NULL after the last string was read.
Str* iterate_str_list(StrList* lst);

// Push a string.
void push_str_list(StrList* lst, Str* str);

// Look at the top of the stack of strings. Returns NULL if the stack (list)
// is empty.
Str* peek_str_list(StrList* lst);

// Remove and free the top of the stack and return the new top of stack.
// Returns NULL when the stack is empty.
Str* pop_str_list(StrList* lst);

// Create a new string. If the str parameter is NULL, then create an empty
// string. Otherwise create a string with the str in it.
Str* create_string(const char* str);

// Create a new string with formatting. Same behavior as create_string()
// except that printf style formatting is used. The parameters cannot be NULL.
Str* create_string_fmt(const char* str, ...);

// Destroy the string. THis is a no-op if GC is being used.a
void destroy_string(Str* ptr);

// Add a character to the end of the string. If the string is empty, then at
// the character as the beginning of the string.
void add_string_char(Str* ptr, int ch);

// Add a const char* string to the end of the string. Parameters may not be
// NULL.
void add_string_str(Str* ptr, const char* str);

// Add a formatted string to the end of the string. Same rules as printf.
void add_string_fmt(Str* ptr, const char* str, ...);

// Reset the internal state the of string to the beginning of it. Do this in
// preparation to iterate the string.
void reset_string(Str* ptr);

// Return the string, one character at a time and return -1 when there are no
// more to return.
int iterate_string(Str* ptr);

// Return a '\0' terminated string for use outside of the data structure. The
// value returned is read-only and should not be free()d or modified outside
// of this API.
const char* raw_string(Str* ptr);

// Simple wrapper to compare two Str data structures. Same rules are strcmp().
int comp_str(Str* s1, Str* s2);

// Simple compare of a Str to a const char* string.
int comp_str_const(Str* s1, const char* s2);
```

## HASH

The hash table uses linear probing where the probing distance is hash & 0x0F. If the result is 0 then the distance is 1. When a hash is deleted, the memory is freed and the tombstone flag is set. When a hash is added, it can be added to a bucket which is a tombstone. The table is full when 3/4 of the buckets are in use. The table is resized and all of the existing hashes are rehashed into the new table. The add function tracks the max number of hops that are needed to insert a new hash. If the hops exceed a certain number, then the hash table should be rehashed, but only if a certain number of adds have taken place to avoid performance problems. Maybe tombstones should be counted instead of hops, but I do not anticipate needing to delete a lot of entries. Rehashing deletes tombstones.

Reference: https://programming.guide/hash-tables-open-addressing.html

### API

```C
typedef enum {
    HASH_OK,    // no errors
    HASH_DUP,   // failure: trying to add a duplicate
    HASH_NF,    // failure: trying to retrieve a non-existant elementa
} HashResult;

// Allocate memory for the hast table data structure using the MEM API above.
HashTable create_hash();

// Destroy the has table. This a no-op when using GC.
void destroy_hash(HashTable table);

// Insert the data given in data and size into the has table using the key.
// Note that the data is re-allocated and copied into the table. Making changes
// to the data after it has been added to the table does not change the data in
// the table.
HashResult insert_hash(HashTable table, const char* key, void* data, size_t size);

// Retrieve the data structure from the table. The data is copied into the
// data parameter. If the size in the table and the size given by the parameter
// don't match then the smaller of the two is taken.
HashResult find_hash(HashTable tab, const char* key, void* data, size_t size);

// Remove a hash table entry and free its memory.
HashResult remove_hash(HashTable tab, const char* key);
```

## CMD

Simple command line parser for C.

This is a simple library that is intended to be used with a language compiler. It does not attempt to be compatible with POSIX or Windows, but it does attempt to be easy to use and complete. Both ease of use for the developer who is writing the program that uses the command line, and for the end-user that is using the program is considered to be a priority.

### Supported features

* A parameter name is the string on the command line that causes the parser to perform some action or that introduces an argument. Parameter names always start with one or more dashes (-) and it can have one or more alphabetical characters or dashes. No digits or other punctuation characters are allowed.
* The general format of a command parameter is ``-name value``. Also ``-name=value``, ``-name:value`` are valid and equivalent. Any punctuation character except ```-``` is recognized as a name/value separator.  A name may have a ```-``` embedded in it, such as ``--some-name`` and be recognized as a valid single parameter name.
  * A parameter name is matched by its length, as well as its content. For example, a parameter named ``-a`` is different from a parameter named ``-abc``. When this is done, the longest match is used.
  * A value can be any valid string that can be passed on the command line. For example, ``-x="a description string"`` is valid for most operating systems and will be passed as a single string to the command line parser. However, the line ``-x=a description string`` will be passed to the program as three separate strings by most operating systems and there is no way to guess what the true intent of the user was.
* All values are handled as strings. If the developer wants to treat a value as, for example a number, then they are responsible for doing the conversion according to their needs.
* Required and optional parameters. The default is optional. It must be marked as required if that is intended.
* Parameter arguments in the form of ``-x=123``, ``-x:123``, ``-x123`` and ``-x 123`` are equivalent. However, something like ``-xabc`` can create an ambiguity. The parser resolves this by accepting the longest possible match. So if the parser knows about ``-x`` and ``-xa``, then the command line argument will be equivalent to ``-xa=bc``, rather than ``-x=abc``. But a command argument like ``-xbcd`` will parse as ``-x=bcd``.
* An option list can have one of 2 forms.
  * In the first form, the list is created in the application without a parameter name. In this case, any stray string found on the command line that does not have a ``-`` in front of it will be added to the list. For example, a list of files, such as ``gcc`` or ``clang`` accepts.
  * In the second form, the list is connected with a parameter name and separated by a comma. For example, ``-x:opt1,opt2,opt3``. When this format is used, then the parameter cannot contain spaces adjacent to the comma character, but spaces can be embedded in an option using quotes, according to the command line rules of the operating system.
* The parameters ``-h``, ``--help``, and ``-?`` are reserved and print the help screen and exit. Note that the parameter ``-hq`` or ``-hx`` are valid stand-alone parameters like any other.

### Unsupported features

* Grouped parameters. For example, ``-abc`` is a single parameter name, and not ``-a -b -c``.
* Mutually required parameters.
* Mutually exclusive parameters.
* Duplicate parameters

### API

```C
// Flags are a bitmask. For example, OR them together, such as
// (CMD_LIST|CMD_REQD) to specify a required list parameter.
typedef enum {
    CMD_NONE = 0x00,
    CMD_REQD = 0x01,
    CMD_LIST = 0x02,
    CMD_BOOL = 0x04,
} CmdFlag;

// Use an opaque handle.
typedef void* CmdLine;

// Create the command line data structure.
CmdLine create_cmd(const char* description);

// Destroy the command line data structure.
void destroy_cmd(CmdLine cmd);

// Add a command line parameter to the parser.
//    parm = the name to recognize on the command line.
//    name = the name to use when retrieving a parameter.
//    dvalue = the default value of the parameter.
//    flags = controls the specify the behavior of the
//            parameter on the command line.
// If this is called with a NULL param and CMD_LIST as a
// flag, then random strings (like file names) will be
// stored under the name.
void add_cmd_item(CmdLine cmd,
                 const char* parm,
                 const char* name,
                 const char* dvalue,
                 unsigned char flags);

// Read the actual command line into the data structure and abort
// the program if there is an error.
void parse_cmd(CmdLine cmd, int argc, char** argv);

// If the name was defined as a list parameter, then reset the iterator.
// Otherwise, simply return without doing anything.
void reset_cmd(CmdLine cmd, const char* name);

// Get the command line string. If the value is defined as a boolean,
// then return NULL for false or a pointer to a blank string for true.
// Otherwise, if the value is defined as a string, then return the
// string if it is defined, or NULL if it is not. If the command was
// defined as a list then return the list iteration.

// return the parameter as a "cooked" string
Str* get_cmd_str(CmdLine cl, const char* name);

// return the parameter as a long int. Note that the user is responsible for
// checking the validity of the number.
long int get_cmd_int(CmdLine cl, const char* name);

// return the parameter as an unsigned long int. Note that the user is
// responsible for checking the validity of the number.
unsigned long int get_cmd_unsigned(CmdLine cl, const char* name);

// return the parameter as a double float
double get_cmd_float(CmdLine cl, const char* name);

// return an iterable list of strings
StrList* get_cmd_list(CmdLine cl, const char* name);

// return the boolean value. The default is always false.
bool get_cmd_bool(CmdLine cl, const char* name);

// return the flag so you can see what the type is.
CmdFlag get_cmd_flag(CmdLine cl, const char* name);
```
