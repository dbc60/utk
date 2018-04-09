#include "cli_command_help.h"

b32 command_execute_help(cli_command_help *me) {
    cli_command_help *help = (cli_command_help*)me;

    // do stuff
    UNREFERENCED(help);
    return TRUE;
}

struct cli_command_help cli_help = {
    0,
    "help",
    "help [-d|--details] <command>\" for command-specific options.",
    "help [-d|--details] <command> for command-specific options. Add '-d' or '--details' for more detailed information.",
    0
};
