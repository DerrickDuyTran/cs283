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
            if (cmd_buff->argc > 1 && chdir(cmd_buff->argv[1]) != 0) {
            }
            break;
        case BI_RC:
            printf("0\n");
            break;
        default:
            break;
    }
    return BI_EXECUTED;
}


//builds a command list 
int build_cmd_list(char *cmd_line, command_list_t *clist) {
    clist->num = 0;
    char *input = strdup(cmd_line);

    char *token = strtok(input, "|");
    while (token != NULL) {
        if (clist->num >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
        char *cleaned_command_line = remove_extra_spaces(token);
        if (strlen(cleaned_command_line) == 0) {
            return WARN_NO_CMDS;
        }
        int rc = build_cmd_buff(cleaned_command_line, &clist->commands[clist->num]);
        if (rc != OK) {
            return rc;
        }
        clist->num++;
        token = strtok(NULL, "|");
    }
    
    if (clist->num == 0) {
        return WARN_NO_CMDS;
    }
    return OK;
}

//frees command list
int free_cmd_list(command_list_t *clist) {
    for (int i = 0; i < clist->num; i++) {
        free_cmd_buff(&clist->commands[i]);
    }
    clist->num = 0;
    return OK;
}

int execute_pipeline(command_list_t *clist) {
    int input = -1; 
    pid_t pids[CMD_MAX];
    int num_pids = 0;

    for (int cmd_num = 0; cmd_num < clist->num; cmd_num++) {
        int fds[2];

        if (cmd_num < clist->num - 1) {
            if (pipe(fds) < 0) {
                return ERR_EXEC_CMD;
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            return ERR_EXEC_CMD;
        }

        if (pid == 0) { 
            if (cmd_num > 0) {
                dup2(input, 0);
                close(input);
            }
            if (cmd_num < clist->num - 1) {
                dup2(fds[1], 1);
                close(fds[1]);
            }
            if (cmd_num > 0) {
                close(input);
            }
            if (cmd_num < clist->num - 1) {
                close(fds[0]);
            }

            execvp(clist->commands[cmd_num].argv[0], clist->commands[cmd_num].argv);
            exit(ERR_EXEC_CMD);
        } else { 
            if (cmd_num > 0) {
                close(input);
            }
            if (cmd_num < clist->num - 1) {
                input = fds[0];
                close(fds[1]);
            }

            pids[num_pids++] = pid;
        }
    }

    for (int i = 0; i < num_pids; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return OK;
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
    command_list_t clist;

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        char *cleaned_command_line = remove_extra_spaces(cmd_buff);
        if (strlen(cleaned_command_line) == 0) {
            printf(CMD_WARN_NO_CMD);
        }
        rc = build_cmd_list(cleaned_command_line, &clist);

        if (rc == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        } else if (rc != OK) {
        }

        if (clist.num == 1) {
            cmd_buff_t *cmd = &clist.commands[0];
            Built_In_Cmds builtin_command = match_command(cmd->argv[0]);
            if (builtin_command != BI_NOT_BI) {
                exec_built_in_cmd(cmd);
            } else {
                pid_t process_id = fork();
                if (process_id < 0) {
                } else if (process_id == 0) {
                    execvp(cmd->argv[0], cmd->argv);
                    exit(ERR_EXEC_CMD);
                } else {
                    waitpid(process_id, NULL, 0);
                }
            }
        } else {
            execute_pipeline(&clist);
        }
    }
    free_cmd_list(&clist);
    free(cmd_buff);
    return OK;
}