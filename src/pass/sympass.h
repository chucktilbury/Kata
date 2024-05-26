/**
 * @file sympass.h
 * 
 * @brief 
 * 
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-27-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _SYMPASS_H_
#define _SYMPASS_H_
#include "ast.h"
#include "symbols.h"

void pre_sym_pass(ast_node* node);
void post_sym_pass(ast_node* node);

#endif  /* _SYMPASS_H_ */
