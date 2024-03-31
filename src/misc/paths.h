/**
 * @file paths.h
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-31-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __PATHS_H__
#define __PATHS_H__

#include "sstrings.h"

void init_paths(const char** names);
void add_path(const char* name);
String* find_file(const char* name);

#endif /* __PATHS_H__ */
