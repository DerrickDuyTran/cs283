#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  remove_extra_spaces
 *    str *:     string to parse
 *    return: parsed string
 */
char *remove_extra_spaces(char *str)
{
  char *end;

  // trim leading space
  while(isspace((unsigned char)*str)) str++;

  // trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  return str;
}

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist) {
    int num_commands = 0;
    int error_state = OK;
    char *cmd_str = strtok(cmd_line, "|");

    while (cmd_str != NULL && error_state == OK) {
        char *trimmed = remove_extra_spaces(cmd_str);

        // allocate memory
        char *cmd_name = (char*) malloc(EXE_MAX * sizeof(char));
        char *cmd_args = (char*) malloc(ARG_MAX * sizeof(char));

        if (cmd_name == NULL || cmd_args == NULL) {
            error_state = ERR_CMD_OR_ARGS_TOO_BIG;
            break;
        }

        // make empty strings for cmd_name, cmd_args
        memset(cmd_name, 0, EXE_MAX);
        memset(cmd_args, 0, ARG_MAX);

        char *curr_pos = NULL;
        char *rest_of_str = strtok_r(trimmed, " ", &curr_pos);

        if (rest_of_str != NULL) {
            // copy executable name 
            strncpy(cmd_name, rest_of_str, EXE_MAX - 1);
        }

        // process commands
        char *curr_cmd = strtok_r(NULL, " ", &curr_pos);
        while (curr_cmd != NULL) {
            strcat(cmd_args, curr_cmd);
            curr_cmd = strtok_r(NULL, " ", &curr_pos);
        }

        // check for errors 
        if (error_state != OK) {
            break;
        }

        // check for max commands
        if (num_commands >= CMD_MAX) {
            error_state = ERR_TOO_MANY_COMMANDS;
            break;
        }

        // save command
        strncpy(clist->commands[num_commands].exe, cmd_name, EXE_MAX - 1);
        strncpy(clist->commands[num_commands].args, cmd_args, ARG_MAX - 1);
        num_commands++;

        // free all mem
        free(cmd_name);
        free(cmd_args);

        cmd_str = strtok(NULL, "|");
    }

    // set the number of commands
    clist->num = num_commands;

    // check for error
    if (error_state != OK) {
        return error_state;
    }

    // check for no commands
    if (num_commands == 0) {
        return WARN_NO_CMDS;
    }

    return OK;
}
