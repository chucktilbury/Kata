
#include <stdio.h>

#include "util.h"

int main(int argc, char** argv) {

    CmdLine cmd = create_cmd_line("This is a test command line.");
    add_cmd(cmd, "-h", "help", "Show the help documentation.", NULL, CMD_HELP);
    add_cmd(cmd, "--plart", "plart", "Make a spicy plart in the middle.", NULL, CMD_LIST); //|CMD_REQD);
    add_cmd(cmd, "--pla", "pla", "Zoom zoom eats glue.", "blubbs", CMD_STR);
    add_cmd(cmd, "-glue-blue", "blue", "When the clock strikes 12.", NULL, CMD_NONE | CMD_LIST);
    add_cmd(cmd, "-v", "verbo", "Set the verbosity level.", "10", CMD_INT);
    add_cmd(cmd, "-x", "ecks", "Set the ecks flag.", NULL, CMD_BOOL);
    add_cmd(cmd, "", "bang", "When bangarang meets the planderflip, there will be trouble.",
            NULL, CMD_REQD | CMD_LIST);

    parse_cmd_line(cmd, argc, argv);

    dump_cmd_line(cmd);

    printf("verbosity is %ld\n", get_cmd_int(cmd, "verbo"));

    return 0;
}
