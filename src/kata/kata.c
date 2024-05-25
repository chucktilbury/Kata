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
#include "sympass.h"

void pre_test_pass(ast_node* node);
void post_test_pass(ast_node* node);

int main(int argc, char** argv) {

    INIT_TRACE;
    SET_TRACE_STATE(false);
    ENTER;

    CmdLine cmd = create_cmd_line("The Kata programming language");
    add_cmd(cmd, "", "file", "Name of file to compile.", NULL, CMD_REQD | CMD_STR);
    parse_cmd_line(cmd, argc, argv);

    // dump_cmd_line(cmd);

    init_parser(get_cmd_raw(cmd, "file"));

    SET_TRACE_STATE(false);
    TRACE("--------------------------------------------");
    TRACE("Begin Parse");
    ast_module* node = parse();
    TRACE("End Parse");
    TRACE("--------------------------------------------");

    SET_TRACE_STATE(true);
    TRACE("--------------------------------------------");
    TRACE("Begin Symbol Traverse");
    traverse_ast(node, pre_sym_pass, post_sym_pass);
    TRACE("End Symbol Traverse");
    TRACE("--------------------------------------------");

    SET_TRACE_STATE(false);

    dump_symtab();

    RETV(0);
}
