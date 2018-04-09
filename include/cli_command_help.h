#ifndef CLI_COMMAND_HELP_H_INCLUDED
#define CLI_COMMAND_HELP_H_INCLUDED

#include "platform.h"


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

struct cli_command_help_vtbl {
    b32(*execute)(cli_command_help *);
    b32 (*parse)(cli_command_help *me, int argc, ch8 **argv);
    const ch8 *(*help_short)(cli_command_help *me);
    const ch8 *(*help_detailed)(cli_command_help *me);
    const ch8 *(*get_name)(cli_command_help *me);
    int (*get_error_code)(cli_command_help *me);
    b32 (*match)(cli_command_help *me, const ch8 *name);
};

// The help-command object
extern struct cli_command_help cli_help;

#endif  // CLI_COMMAND_HELP_H_INCLUDED
