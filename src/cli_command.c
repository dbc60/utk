#include "cli_command.h"
#include <string.h>


static b32 execute(cli_command *me);
static b32 parse(cli_command *me, int argc, ch8 **argv);
static const ch8 *get_help_short(const cli_command *me);
static const ch8 *get_help_detailed(const cli_command *me);
static const ch8 *get_name(const cli_command *me);
static int get_error_code(const cli_command *me);
static b32 match(const cli_command *me, const ch8 *name_command);

static cli_command_vtbl vtbl_default = {
    execute,
    parse,
    get_help_short,
    get_help_detailed,
    get_name,
    get_error_code,
    match
};

void
cli_command_init(cli_command *me, cli_command_vtbl *vtbl, const ch8 *name,
                 const ch8 *help_short, const ch8 *help_detailed) {
    me->vtbl = vtbl;
    me->name = name;
    me->help_short = help_short;
    me->help_detailed = help_detailed;
    me->error_code = 0;
}


/** @brief public functions */
b32
cli_command_execute(cli_command *me) {
    return me->vtbl->execute(me);
}


b32
cli_command_parse(cli_command *me, int argc, ch8 **argv) {
    return me->vtbl->parse(me, argc, argv);
}


const ch8 *
cli_command_get_help_short(const cli_command *me) {
    return me->vtbl->help_short(me);
}


const ch8 *
cli_command_help_detailed(const cli_command *me) {
    return me->vtbl->help_detailed(me);
}


const ch8 *
cli_command_get_name(const cli_command *me) {
    return me->vtbl->get_name(me);
}


int
cli_command_get_error_code(const cli_command *me) {
    return me->vtbl->get_error_code(me);
}


b32
cli_command_match(const cli_command *me, const ch8 *name) {
    return me->vtbl->match(me, name);
}


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


// return a short help string
const
ch8 *get_help_short(const cli_command *me) {
    return me->help_short;
}


// return the command name with a description of its arguments
const
ch8 *get_help_detailed(const cli_command *me) {
    return me->help_detailed;
}


// return the name of the command
const
ch8 *get_name(const cli_command *me) {
    return me->name;
}


// return a numeric code representing the result of executing the command
int
get_error_code(const cli_command *me) {
    return me->error_code;
}


// return TRUE if the string matches the command name, and FALSE otherwise.
b32
match(const cli_command *me, const ch8 *name) {
    return 0 == strcmp(me->name, name);
}
