#ifndef SYNTAX
#define SYNTAX

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

#include "tokenlist.h"
#include "config.h"

int syntax_analysis(token_list *t);

void shell_cmd(token_list **t, int *status);
void condition_cmd(token_list **t, int *status);
void cmd(token_list **t, int *status);

void io_redirect(token_list **t, int *status);
void in_redirect(token_list **t, int *status);
void out_redirect(token_list **t, int *status);

void conveyor(token_list **t, int *status, int *conv_desc);
void simple_cmd(token_list **t, int *status, int *conv_desc);
void cmd_exec(token_list **t, char *command, char **argv, int *conv_desc);

int iscmdtoken(token_list *token);

/*
<shell_cmd> ::= <condition_cmd> { [; | &] <shell_cmd> } { [; | &] }
<condition_cmd> ::= <cmd> { [&& | ||] <condition_cmd> } 
<cmd> ::= {<io_redirect>}<conveyor> | <conveyor>{<io_redirect>} | (<shell_cmd>)
<io_redirect> ::= {<in_redirect>}<out_redirect> | {<out_redirect>}<in_redirect>
<in_redirect> ::= '<' <file_name>
<out_redirect> ::= '>' <file_name>
<conveyor> ::= <simple_cmd>{'|'<conveyor>}
<simple_cmd> ::= <cmd_name><argv>
*/



#endif // SYNTAX
