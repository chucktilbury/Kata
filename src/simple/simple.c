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
#include "util.h"
//#include "scan.h"
#include "parse.h"
#include "trace.h"
//#include "pass.h"

void pre_test_pass(ast_node* node);
void post_test_pass(ast_node* node);

int main(int argc, char** argv) {

    PUSH_TRACE_STATE(false);
    ENTER;

    CmdLine cmd = create_cmd_line("The Simple programming language");
    add_cmd(cmd, "", "file", "Name of file to compile.", NULL, CMD_REQD | CMD_STR);
    parse_cmd_line(cmd, argc, argv);

    init_parser(get_cmd_raw(cmd, "file"));

    TRACE("--------------------------------------------");
    TRACE("Begin Parse");
    ast_module* node = parse();
    TRACE("End Parse");
    TRACE("--------------------------------------------");

    SET_TRACE_STATE(true);
    TRY {
        TRACE("Begin Traverse");
        traverse_ast(node, pre_test_pass, post_test_pass);
        TRACE("End Traverse");
    }
    ANY_EXCEPT() {
        fprintf(stderr, "AST Traverse exception: %s\n", EXCEPTION_MSG);
        RETV(1);
    }
    FINAL
    TRACE("--------------------------------------------");
    POP_TRACE_STATE();

    RETV(0);
}