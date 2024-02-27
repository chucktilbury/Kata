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

void test_pass(ast_node* node);

int main(int argc, char** argv) {

    ENTER;
    CmdLine cmd = create_cmd_line("The Simple programming language");
    add_cmd(cmd, "", "file", "Name of file to compile.", NULL, CMD_REQD | CMD_STR);
    parse_cmd_line(cmd, argc, argv);

    init_scanner(get_cmd_raw(cmd, "file"));

    TRACE("--------------------------------------------");
    TRACE("Begin Parse");
    ast_module* node = parse();
    TRACE("End Parse");
    TRACE("--------------------------------------------");

    TRY {
        TRACE("Begin Traverse");
        traverse_ast(node, test_pass);
        TRACE("End Traverse");
    }
    ANY_EXCEPT() {
        fprintf(stderr, "AST Traverse exception: %s\n", EXCEPTION_MSG);
        RETV(1);
    }
    FINAL
    TRACE("--------------------------------------------");

    RETV(0);
}