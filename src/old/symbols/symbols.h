#ifndef SYMBOLS_H
#define SYMBOLS_H

typedef void* symbol;

symbol create_symbol(const char* name);
symbol get_symbol(const char* name);
void set_symbol_attr(symbol sym, const char* name, void* attr, size_t size);
void* get_symbol_attr(symbol sym, const char* name);

#endif
