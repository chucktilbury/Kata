/**
 * @file simple.c
 *
 * @brief Implementation of the main function and supporting routines for
 * the compiler.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-15-2024
 * @copyright Copyright (c) 2024
 */
//#include "util.h"
//#include "scan.h"
#include "parse.h"
#include "trace.h"
//#include "pass.h"
#include "command_line.h"

void pre_test_pass(ast_node* node);
void post_test_pass(ast_node* node);

int main(int argc, char** argv) {

    PUSH_TRACE_STATE(true);
    ENTER;

    CmdLine cmd = create_cmd_line("The Kata programming language");
    add_cmd(cmd, "", "file", "Name of file to compile.", NULL, CMD_REQD | CMD_STR);
    parse_cmd_line(cmd, argc, argv);

    // dump_cmd_line(cmd);

    init_parser(get_cmd_raw(cmd, "file"));

    TRACE("--------------------------------------------");
    TRACE("Begin Parse");
    ast_module* node = parse();
    TRACE("End Parse");
    TRACE("--------------------------------------------");

    SET_TRACE_STATE(true);
    TRACE("Begin Traverse");
    traverse_ast(node, pre_test_pass, post_test_pass);
    TRACE("End Traverse");
    TRACE("--------------------------------------------");
    POP_TRACE_STATE();

    RETV(0);
}