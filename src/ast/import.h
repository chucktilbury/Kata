/**
 * @file import.h
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-26-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __IMPORT_H__
#define __IMPORT_H__
#include "ast.h"

/**
 * import_statement
 *      = 'import' SYMBOL ('as' SYMBOL)?
 *
 */
typedef struct _ast_import_statement_ {
    ast_node node;
    Token* name;                 // optional alternative name
    Token* mod;                  // module to import
    struct _ast_module_* module; // imported module
} ast_import_statement;

void traverse_import_statement(ast_import_statement* node, PassFunc pre, PassFunc post);

#endif /* __IMPORT_H__ */
