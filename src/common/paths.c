/**
 * @file paths.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-31-2024
 * @copyright Copyright (c) 2024
 */
#include <errno.h>
//#include <linux/limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "errors.h"
#include "link_list.h"
#include "sstrings.h"
#include "trace.h"

#ifndef PATH_MAX
#define PATH_MAX FILENAME_MAX
#endif 

static LinkList* paths;

static inline bool is_file(const char* name) {

    struct stat sb;
    stat(name, &sb);
    return ((sb.st_mode & S_IFMT) == S_IFREG);
}

/**
 * @brief Find a real file to open using stat(). Returns the full path of
 * the file.
 *
 * @param name
 * @return String*
 */
String* find_file(const char* name) {

    ENTER;

    char tmp_buf[PATH_MAX];
    char temp[PATH_MAX];

    TRACE("trying: %s", name);
    if(is_file(name)) {
        realpath(name, tmp_buf);
        TRACE("found: %s", tmp_buf);
        RETV(create_string(tmp_buf));
    }
    else {
        void* mark = NULL;
        String* p  = NULL;
        while(NULL != (p = iter_link_list(paths, &mark))) {
            memset(temp, 0, sizeof(temp));
            strncpy(temp, raw_string(p), sizeof(temp));
            strncat(temp, name, sizeof(temp) - strlen(temp) - strlen(name) - 1);

            TRACE("trying: %s", temp);
            if(is_file(temp)) {
                realpath(temp, tmp_buf);
                TRACE("found: %s", tmp_buf);
                RETV(create_string(tmp_buf));
            }
            else {
                strncat(temp, ".k", sizeof(temp) - strlen(temp) - strlen(".k") - 1);
                TRACE("trying: %s", temp);
                if(is_file(temp)) {
                    realpath(temp, tmp_buf);
                    TRACE("found: %s", tmp_buf);
                    RETV(create_string(tmp_buf));
                }
            }
        }
    }
    fatal_error("Cannot find input file: %s", name);
    RETV(NULL);
}

void add_path(const char* name) {

    ENTER;

    char buffer[PATH_MAX];

    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, name, sizeof(buffer));

    // make sure that it has a '/' on the end.
    char* ptr = strrchr(buffer, '/');
    if(*(ptr + 1) != '\0')
        strncat(buffer, "/", sizeof(buffer) - (strlen(buffer) + 1));

    TRACE("%s", buffer);
    append_link_list(paths, create_string(buffer));
    RET;
}

void init_paths(const char** names) {

    ENTER;
    paths = create_link_list();

    for(int i = 0; names[i] != NULL; i++)
        add_path(names[i]);

    RET;
}
