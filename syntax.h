#ifndef SYNTAX
#define SYNTAX

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

int syntax_analysis(token_list *t); 
/* 
 * @function Syntax analisys of token list
 * @param *t - input token list
 * @return 0 if syntax is correct, error code if not
 */

/* Syntax analisys is based on Recursive descent parsing method: every function is definition from grammar.
 * Grammar:
 * <shell_cmd> ::= <condition_cmd> { [; | &] <shell_cmd> } { [; | &] }
 * <condition_cmd> ::= <cmd> { [&& | ||] <condition_cmd> } 
 * <cmd> ::= {<io_redirect>}<conveyor> | <conveyor>{<io_redirect>} | (<shell_cmd>)
 * <io_redirect> ::= {<in_redirect>}<out_redirect> | {<out_redirect>}<in_redirect>
 * <in_redirect> ::= '<' <file_name>
 * <out_redirect> ::= '>' <file_name>
 * <conveyor> ::= <simple_cmd>{'|'<conveyor>}
 * <simple_cmd> ::= <cmd_name><argv>
 */

void shell_cmd(token_list **t, int *status); 
/* 
 * @function shell_cmd from grammar
 * @param **t - input token list
 * @param *status - error code
 */

void condition_cmd(token_list **t, int *status);
/* 
 * @function condition_cmd from grammar
 * @param **t - input token list
 * @param *status - error code
 */

void cmd(token_list **t, int *status);
/* 
 * @function cmd from grammar
 * @param **t - input token list
 * @param *status - error code
 */

void io_redirect(token_list **t, int *status);
/* 
 * @function io_redirect from grammar
 * @param **t - input token list
 * @param *status - error code
 */

void in_redirect(token_list **t, int *status);
/* 
 * @function in_redirect from grammar
 * @param **t - input token list
 * @param *status - error code
 */

void out_redirect(token_list **t, int *status);
/* 
 * @function out_redirect from grammar
 * @param **t - input token list
 * @param *status - error code
 */

void conveyor(token_list **t, int *status);
/* 
 * @function conveyor from grammar
 * @param **t - input token list
 * @param *status - error code
 */

void conv_exec(token_list **t, int *status, int *conv_desc);
/* 
 * @function Executes commands from conveyor 
 * @param **t - input token list
 * @param *status - error code
 * @param *conv_desc - file descriptor for conveyor command input/output
 */

void simple_cmd(token_list **t, int *status, int *conv_desc);
/* 
 * @function Executes commands from conveyor (stands for simple_command from grammar)
 * @param **t - input token list
 * @param *status - error code
 * @param *conv_desc - file descriptor for conveyor command input/output
 */

void cmd_exec(token_list **t, char *command, char **argv, int *conv_desc, int *status);
/* 
 * @function Executes commands from conveyor 
 * @param **t - input token list
 * @param *command - command to shell
 * @param **argv - arguments for shell command
 * @param *conv_desc - file descriptor for conveyor command input/output
 * @param *status - finish status
 */

int iscmdtoken(token_list *token);
/* 
 * @function Check if token is command token (|, ||, &, &&, <, >, >>, ;, (, ))
 * @param *token - token to check
 * @return 1 - is command token, 0 - not
 */

int isbgcmd(token_list *token);
/* 
 * @function Check if process should run in background
 * @param *token - beginning of shell command
 * @return 1 - is background command, 0 - not
 */

#endif // SYNTAX
