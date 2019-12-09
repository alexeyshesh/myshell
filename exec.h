#ifndef EXEC
#define EXEC

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#include "tokenlist.h"
#include "config.h"
#include "syntax.h"


void cmd_exec(token_list **t, int *status, int *conv_desc);
/* 
 * @function Executes command from shell 
 * @param **t - input token list
 * @param *conv_desc - file descriptor for conveyor command input/output
 * @param *status - finish status
 */

void simple_cmd_exec(token_list **t, char *command, char **argv, int *conv_desc, int *status, int *exec_status);
/* 
 * @function Executes commands from conveyor 
 * @param **t - input token list
 * @param *command - command to shell
 * @param **argv - arguments for shell command
 * @param *conv_desc - file descriptor for conveyor command input/output
 * @param *status - finish status
 * @param *exec_status - execution status
 */

void conv_exec(token_list **t, int *status, int *conv_desc, int *exec_status);
/* 
 * @function Executes commands from conveyor 
 * @param **t - input token list
 * @param *status - error code
 * @param *conv_desc - file descriptor for conveyor command input/output
 * @param *exec_status - execution status
 */


void simple_cmd(token_list **t, int *status, int *conv_desc, int *exec_status);
/* 
 * @function Executes commands from conveyor (stands for simple_command from grammar)
 * @param **t - input token list
 * @param *status - error code
 * @param *conv_desc - file descriptor for conveyor command input/output
 * @param *exec_status - execution status
 */

void next_cmd(token_list **t);
/* 
 * @function put pointer to the next command from line
 * @param **t - input token list
 */

#endif // EXEC