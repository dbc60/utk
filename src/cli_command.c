#include "cli_command.h"
#include <string.h>


static const ch8 *get_help_short(const cli_command *me);
static const ch8 *get_help_detailed(const cli_command *me);
static const ch8 *get_name(const cli_command *me);
static int get_error_code(const cli_command *me);
static b32 match(const cli_command *me, const ch8 *name_command);

cli_command_vtbl vtbl_cli_command = {
    NULL,
    NULL,
    get_help_short,
    get_help_detailed,
    get_name,
    get_error_code,
    match
};


void
cli_command_init(cli_command *me) {
    me->vtbl = &vtbl_cli_command;
    me->name = "";
    me->help_short = "";
    me->help_detailed = "";
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
