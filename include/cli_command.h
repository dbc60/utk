#ifndef CLI_COMMAND_H_INCLUDED
#define CLI_COMMAND_H_INCLUDED

#include "platform.h"

struct cli_command_vtbl;
typedef struct cli_command_vtbl cli_command_vtbl;

// Here's a notional definition of a command object
struct cli_command_vtbl;
struct cli_command {
    cli_command_vtbl *vtbl;
    const ch8 *name; // the name of the command
    const ch8 *help_short;
    const ch8 *help_detailed;
    int error_code;
};
typedef struct cli_command cli_command;

struct cli_command_vtbl {
    b32 (*execute)(struct cli_command *me);
    b32 (*parse)(struct cli_command *me, int argc, ch8 **argv);
    const ch8 *(*help_short)(const struct cli_command *me);
    const ch8 *(*help_detailed)(const struct cli_command *me);
    const ch8 *(*get_name)(const struct cli_command *me);
    int (*get_error_code)(const struct cli_command *me);
    b32 (*match)(const struct cli_command *me, const ch8 *name);
};
typedef struct cli_command_vtbl cli_command_vtbl;


void cli_command_init(cli_command *me,
                      cli_command_vtbl *vtbl,
                      const ch8 *name,
                      const ch8 *help_short,
                      const ch8 *help_detailed);
b32 cli_command_execute(cli_command *me);
b32 cli_command_parse(cli_command *me, int argc, ch8 **argv);
const ch8 *cli_command_help_short(const cli_command *me);
const ch8 *cli_command_help_detailed(const cli_command *me);
const ch8 *cli_command_get_name(const cli_command *me);
int cli_command_get_error_code(const cli_command *me);
b32 cli_command_match(const cli_command *me, const ch8 *name_command);

#endif  // CLI_COMMAND_H_INCLUDED
