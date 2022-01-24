#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct _table {
    void** table;
    size_t size;
} table_t;

table_t* create_table(size_t size);
void destroy_table(table_t* tab);

void set_table_val(table_t* tab, size_t index, void* data, size_t size);
void set_table_int(table_t* tab, size_t index, int val);
void set_table_str(table_t* tab, size_t index, const char* str);
void set_table_ptr(table_t* tab, size_t index, void* data);

void* get_table(table_t* tab, size_t index);

#endif
