/**
 * @file simple.c
 *
 * @brief Implementation of the main function and supporting routines for the
 * compiler.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-15-2024
 * @copyright Copyright (c) 2024
 */
#include "util.h"
#include "scanner.h"
#include "parser.h"

int main(int argc, char** argv) {

    CmdLine cmd = create_cmd_line("The Simple programming language");
    add_cmd(cmd, "", "file", "Name of file to compile.", NULL, CMD_REQD | CMD_STR);
    parse_cmd_line(cmd, argc, argv);

    init_scanner();
    open_file(get_cmd_raw(cmd, "file"));

    //AstNode* node = module();

    return 0;
}