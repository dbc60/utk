#include "cli_command_help.h"
#include "cli_command.h"

static b32 execute(cli_command *me);
static b32 parse(cli_command *me, int argc, ch8 **argv);
static void command_register(const cli_command *cmd);

static cli_command_help_vtbl vtbl_cli_command_help;

void cli_command_help_init(cli_command_help *me) {
    *((cli_command_vtbl*)&vtbl_cli_command_help) = vtbl_cli_command;
    vtbl_cli_command_help.execute = execute;
    vtbl_cli_command_help.parse = parse;
    vtbl_cli_command_help.command_register = command_register;
    me->vtbl = &vtbl_cli_command_help;
    me->name = "help";
    me->help_short = "help short placeholder";
    me->help_detailed = "help detailed placeholder";
    me->error_code = 0;
}

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


/** @brief "virtual" functions in the vtable */

b32
execute(cli_command *me) {
    me->error_code = 0;
    return TRUE;
}


// parse a cli_command-line - return TRUE for success, FALSE otherwise.
b32
parse(cli_command *me, int argc, ch8 **argv) {
    int compare_result;

    UNREFERENCED(argc);

    // each command has its own name - match it.
    compare_result = strcmp(me->name, argv[1]);
    return (0 == compare_result);
}

void
command_register(const cli_command *cmd) {
    UNREFERENCED(cmd);
}
