#ifndef CLI_COMMAND_HELP_H_INCLUDED
#define CLI_COMMAND_HELP_H_INCLUDED

#include "platform.h"
#include "cli_command.h"

struct cli_command_help_vtbl;
typedef struct cli_command_help_vtbl cli_command_help_vtbl;

struct cli_command_help {
    cli_command_help_vtbl *vtbl;
    const ch8 *name;
    const ch8 *help_short;
    const ch8 *help_detailed;
    int error_code;
    // other fields as needed
};
typedef struct cli_command_help cli_command_help;

typedef void (*pfn_command_register)(const cli_command *cmd);

struct cli_command_help_vtbl {
    pfn_execute execute;
    pfn_parse parse;
    pfn_help_short help_short;
    pfn_help_detailed help_detailed;
    pfn_get_name get_name;
    pfn_get_error_code get_error_code;
    pfn_match match;
    pfn_command_register command_register;
};

// The help-command object
extern struct cli_command_help cli_help;
void cli_command_help_init(cli_command_help *me);

#endif  // CLI_COMMAND_HELP_H_INCLUDED
