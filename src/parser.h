#ifndef PARSER_H
#define PARSER_H

ast_t* init_parser(const char* fname);
int parse(ast_t* root);
void recover_from_error();

#endif
