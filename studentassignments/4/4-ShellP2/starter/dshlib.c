#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

//removes leading, trailing, and spaces between tokens, keeps quoted strings 
char *remove_extra_spaces(char *str) {
    char *source_ptr = str, *dest_ptr = str;
    while (*source_ptr && isspace((unsigned char)*source_ptr))
        source_ptr++;
    int inside_quote = 0;
    while (*source_ptr) {
        if (*source_ptr == '"') {
            inside_quote = !inside_quote;
            *dest_ptr++ = *source_ptr++;
        } else if (!inside_quote && isspace((unsigned char)*source_ptr)) {
            *dest_ptr++ = ' ';
            while (*source_ptr && isspace((unsigned char)*source_ptr))
                source_ptr++;
        } else {
            *dest_ptr++ = *source_ptr++;
        }
    }
    if (dest_ptr > str && isspace((unsigned char)*(dest_ptr - 1)))
        dest_ptr--;
    *dest_ptr = '\0';
    return str;
}

//frees memory of cmd buff
int free_cmd_buff(cmd_buff_t *cmd_buff) {
    free(cmd_buff->_cmd_buffer);
    cmd_buff->_cmd_buffer = NULL;
    return OK;
}

//parses the command line into arguments, handling spaces and quoted strings, and stores them in cmd_buff
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    char *cleaned_cmd_line = remove_extra_spaces(cmd_line);
    cmd_buff->_cmd_buffer = strdup(cleaned_cmd_line);
    if (!cmd_buff->_cmd_buffer)
        return ERR_MEMORY;
    cmd_buff->argc = 0;
    char *current_ptr = cmd_buff->_cmd_buffer;
    while (*current_ptr != '\0' && cmd_buff->argc < CMD_ARGV_MAX - 1) {
        if (*current_ptr == ' ') {
            current_ptr++;
        } else if (*current_ptr == '"') {
            current_ptr++;
            char *token_ptr = current_ptr;
            while (*current_ptr && *current_ptr != '"')
                current_ptr++;
            if (*current_ptr == '"') {
                *current_ptr = '\0';
                current_ptr++;
            }
            cmd_buff->argv[cmd_buff->argc++] = token_ptr;
        } else {
            char *token_ptr = current_ptr;
            while (*current_ptr && *current_ptr != ' ')
                current_ptr++;
            if (*current_ptr != '\0') {
                *current_ptr = '\0';
                current_ptr++;
            }
            cmd_buff->argv[cmd_buff->argc++] = token_ptr;
        }
    }
    cmd_buff->argv[cmd_buff->argc] = NULL;
    return OK;
}

//matches command to a built-in command
Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0) return BI_CMD_EXIT;
    if (strcmp(input, "cd") == 0) return BI_CMD_CD;
    if (strcmp(input, "dragon") == 0) return BI_CMD_DRAGON;
    if (strcmp(input, "rc") == 0) return BI_RC;
    return BI_NOT_BI;
}

//executes a built-in command based on cmd_buff
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd_buff) {
    Built_In_Cmds builtin_command = match_command(cmd_buff->argv[0]);
    switch (builtin_command) {
        case BI_CMD_EXIT:
            exit(0);
        case BI_CMD_CD:
            if (cmd_buff->argc > 1 && chdir(cmd_buff->argv[1]) != 0)
                perror("chdir");
            break;
        case BI_RC:
            printf("0\n");
            break;
        default:
            break;
    }
    return BI_EXECUTED;
}

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop() {
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    int rc = 0;
    cmd_buff_t cmd;

    // TODO IMPLEMENT MAIN LOOP
    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
        
        // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        char *cleaned_command_line = remove_extra_spaces(cmd_buff);
        if (strlen(cleaned_command_line) == 0) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }
        rc = build_cmd_buff(cleaned_command_line, &cmd);
        
        // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
        // the cd command should chdir to the provided directory; if no directory is provided, do nothing
        Built_In_Cmds builtin_command = match_command(cmd.argv[0]);
        if (builtin_command != BI_NOT_BI) {
            exec_built_in_cmd(&cmd);
        } else {
        // TODO IMPLEMENT if not built-in command, fork/exec as an external command
        // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
            pid_t process_id = fork();
            if (process_id < 0) {
                perror("fork");
            } else if (process_id == 0) {
                execvp(cmd.argv[0], cmd.argv);
                perror("execvp");
                exit(ERR_EXEC_CMD);
            } else {
                int child_status;
                wait(&child_status);
            }
        }
        
        free_cmd_buff(&cmd);
    }

    free(cmd_buff);
    return OK;
}
