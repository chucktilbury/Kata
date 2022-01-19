#ifndef PARSER_H
#define PARSER_H

typedef enum {
    PT_NAMESPACE,
    PT_CLASS,
} obj_type_t;

typedef enum {
    PUB_SCOPE,
    PRIV_SCOPE,
    PROT_SCOPE,
} scope_t;

typedef struct {
    obj_type_t type;
} object_type;

int parse();
void recover_from_error();
void init_parser(const char* fname);

#endif
