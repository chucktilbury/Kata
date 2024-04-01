/**
 * @file parser_test.c
 *
 * @brief Test the parser without passes on the AST.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-15-2024
 * @copyright Copyright (c) 2024
 */
#include "util.h"
//#include "scanner.h"
#include "ast.h"
#include "parse.h"
#include "trace.h"

int main(int argc, char** argv) {

    ENTER;
    CmdLine cmd = create_cmd_line("The Simple programming language");
    add_cmd(cmd, "", "file", "Name of file to compile.", NULL, CMD_REQD | CMD_STR);
    parse_cmd_line(cmd, argc, argv);

    TRACE("Begin Parse");
    ast_module* node = parse(get_cmd_raw(cmd, "file"));
    TRACE("End Parse\n");

    // TRY {
    //     TRACE("Begin Traverse");
    //     traverse_ast(node);
    //     TRACE("End Traverse\n");
    // }
    // ANY_EXCEPT() {
    //     fprintf(stderr, "AST Traverse exception: %s\n", EXCEPTION_MSG);
    //     RETV(1);
    // }
    // FINAL

    RETV(0);
}
