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
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "link_list.h"
#include "sstrings.h"
#include "trace.h"
#include "errors.h"

static LinkList* paths;

/**
 * @brief Search the path to find the file. Return a fully qualified file name.
 *
 * @param name
 * @return const char*
 *
 */
String* find_file(const char* name) {

    ENTER;
    char temp[PATH_MAX];
    char tmp_buf[PATH_MAX];
    String* p;
    void* mark = NULL;

    TRACE("trying: %s", name);
    if(NULL != realpath(name, tmp_buf)) {
        TRACE("found: %s", tmp_buf);
        RETV(create_string(tmp_buf));
    }
    else {
        while(NULL != (p = iter_link_list(paths, &mark))) {
            memset(temp, 0, sizeof(temp));
            strncpy(temp, raw_string(p), sizeof(temp));
            strncat(temp, name, sizeof(temp)-strlen(temp)-strlen(name)-1);

            TRACE("trying: %s", temp);
            if(NULL != realpath(temp, tmp_buf)) {
                TRACE("found: %s", tmp_buf);
                RETV(create_string(tmp_buf));
            }
            else {
                // try appending the ".simp"
                strncat(temp, ".simp", sizeof(temp)-strlen(temp)-strlen(".simp")-1);
                if(NULL != realpath(temp, tmp_buf)) {
                    TRACE("found: %s", tmp_buf);
                    RETV(create_string(tmp_buf));
                }
            }
        }
    }

    // reach here because the name could not be located.
    fatal_error("Cannot find input file: %s", name);
    RETV(NULL); // keep the compiler happy
}

void add_path(const char* name) {

    ENTER;

    char buffer[PATH_MAX];

    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, name, sizeof(buffer));

    // make sure that it has a '/' on the end.
    char* ptr = strrchr(buffer, '/');
    if(*(ptr+1) != '\0')
        strncat(buffer, "/", sizeof(buffer) - (strlen(buffer)+1));

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

