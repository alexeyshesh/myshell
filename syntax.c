#include "syntax.h"

int iscmdtoken(token_list *token) {
    if (!token)
        return 1;
    return !strcmp(">", token->token) || !strcmp("<", token->token) || !strcmp("&", token->token) || 
           !strcmp("(", token->token) || !strcmp(")", token->token) || !strcmp("|", token->token) ||
           !strcmp(">>", token->token) || !strcmp("&&", token->token) || !strcmp("||", token->token) ||
           !strcmp(";", token->token);
}

int syntax_analysis(token_list *t) {
    int status = 0;
    shell_cmd(&t, &status);
    if (t != NULL) {
        status = -1;
    }
    return status;
}

void shell_cmd(token_list **t, int *status) {
    condition_cmd(t, status);
    if (!(*t) || *status != 0)
        return;
    if ((*t)->next) {
        if (strcmp(";", (*t)->token) == 0 || strcmp("&", (*t)->token) == 0) {
            *t = (*t)->next;
            shell_cmd(t, status);
            return;
        } else {
            *t = (*t)->next;
            return;
        }
    }
    
}

void condition_cmd(token_list **t, int *status) {
    cmd(t, status);
    if (!(*t) || *status != 0)
        return;
    if ((*t)->next) {
        if (strcmp("&&", (*t)->token) == 0 || strcmp("||", (*t)->token) == 0) {
            *t = (*t)->next;
            condition_cmd(t, status);
            return;
        } else {
            return;
        }
    }
}

void cmd(token_list **t, int *status) {
    if (!(*t) || *status != 0) {
        *status = -3;
        return;
    }

    if (strcmp((*t)->token, "(") == 0) {
        (*t) = (*t)->next;
        shell_cmd(t, status);
        if (!(*t) || *status != 0)
            return;
        if (strcmp((*t)->token, ")") == 0) {
            (*t) = (*t)->next;
            return;
        } else {
            *status = -4;
            return;
        }
    }
    
    int conv_desc[2]; // file descriptors for conveyor
    /* save descriptors before redirection */
    int old_in_desc = dup(0), old_out_desc = dup(1);
    if (strcmp((*t)->token, ">") == 0 || strcmp((*t)->token, "<") == 0 || strcmp((*t)->token, ">>") == 0) {
        

        io_redirect(t, status);

        if (*t) {
            if (strcmp((*t)->token, ">") == 0 || strcmp((*t)->token, "<") == 0 || strcmp((*t)->token, ">>") == 0)
                cmd(t, status);
        }
    }
    conveyor(t, status, conv_desc);
    /* if i/o files descriptors changed return them to the old state */
    dup2(old_in_desc, 0);
    dup2(old_out_desc, 1);
    /* 

    conveyor(t, status);
    if (!(*t) || *status != 0)
        return;
    if (strcmp((*t)->token, ">") == 0 || strcmp((*t)->token, "<") == 0) {
        io_redirect(t, status);
        return;
    }

    */

}

void io_redirect(token_list **t, int *status) {

    if (!(*t) || *status != 0) {
        *status = -5;
        return;
    }
    if (strcmp((*t)->token, "<") == 0) {
        in_redirect(t, status);
        if (*t) {
            if (strcmp((*t)->token, ">") == 0 || strcmp((*t)->token, ">>") == 0) {
                out_redirect(t, status);
            }
        } 
        return;
    }
    if (strcmp((*t)->token, ">") == 0 || strcmp((*t)->token, ">>") == 0) {
        out_redirect(t, status);
        if (*t) {
            if (strcmp((*t)->token, "<") == 0) {
                in_redirect(t, status);
            }
        }
        return;
    }
    
    *status = -6;
}

void in_redirect(token_list **t, int *status) {
    if (!(*t) || *status != 0) {
        *status = -7;
        return;
    }

    if (strcmp((*t)->token, "<") == 0) {
        (*t) = (*t)->next;

        /* (*t)->token is filename. So some processing */
        int in_desc;
        if ((in_desc = open((*t)->token, O_RDONLY)) != -1) {
            dup2(in_desc, 0);
            close(in_desc);
        } else {
            printf("Can not open file `%s`\n", (*t)->token);
            *status = -11;
            return;
        }
            
        (*t) = (*t)->next;
    }
}

void out_redirect(token_list **t, int *status) {
    if (!(*t) || *status != 0) {
        *status = -8;
        return;
    }

    if (strcmp((*t)->token, ">") == 0 || strcmp((*t)->token, ">>") == 0) {
        int out_desc, open_flags;
        if (strcmp((*t)->token, ">") == 0) {
            open_flags = O_RDWR | O_CREAT | O_TRUNC;
        } else {
            open_flags = O_RDWR | O_CREAT | O_APPEND;
        }
        (*t) = (*t)->next;

        /* (*t)->token is filename. So some processing */
        
        if ((out_desc = open((*t)->token, open_flags, 0666)) != -1) {
            dup2(out_desc, 1);
            close(out_desc);
        } else {
            printf("Can not open file `%s`\n", (*t)->token);
            *status = -12;
            return;
        }

        (*t) = (*t)->next;
        return;
    }
}

void conveyor(token_list **t, int *status, int *conv_desc) {
    if (!(*t) || *status != 0) {
        *status = -9;
        return;
    }

    simple_cmd(t, status, conv_desc);
    if ((*t)) {
        if (strcmp((*t)->token, "|") == 0) {
            (*t) = (*t)->next;
            conveyor(t, status, conv_desc);
        }
    }
}

void simple_cmd(token_list **t, int *status, int *conv_desc) {
    if (!(*t) || *status != 0) {
        *status = -10;
        return;
    }

    if (strcmp((*t)->token, ">") == 0 || strcmp((*t)->token, "<") == 0 || strcmp((*t)->token, ">>") == 0) {
        return;
    }

    char *command = (*t)->token;
    int size = 1, argc = 0;
    char **argv = malloc(sizeof(char*) * size);
    if (!argv)
        fatal_error(HEAP_OVERFLOW);
    while (!iscmdtoken(*t)) {
        if (argc == size - 1) {
            argv = realloc(argv, sizeof(char*) * (size *= 2));
            if (!argv) 
                fatal_error(HEAP_OVERFLOW);
        }
        argv[argc] = (*t)->token;
        (*t) = (*t)->next;
        argc++; 
    }
    argv[argc] = NULL;

    cmd_exec(t, command, argv, conv_desc);

    free(argv);
}

void cmd_exec(token_list **t, char *command, char **argv, int *conv_desc) {

    if (strcmp("cd", command) == 0) {
        if (argv[1] == NULL)
            argv[1] = getenv("HOME");
        if (chdir(argv[1]) != 0) {
            printf("%s: no such file or directory\n", argv[1]);
        }
        return;
    }

    int child;
    if (pipe(conv_desc) < 0) 
        fatal_error(PIPE_ERR);

    if ((child = fork()) < 0) {
        fatal_error(PROC);
    } else if (child == 0) {
        signal(SIGINT, SIG_DFL);
        if (*t && strcmp((*t)->token, "|") == 0)
            dup2(conv_desc[1], 1);
        close(conv_desc[0]);
        close(conv_desc[1]);
        if (execvp(command, argv) == -1)
            printf("%s: command not found\n", command);
        exit(0);
    } else {
        dup2(conv_desc[0], 0);
        close(conv_desc[0]);
        close(conv_desc[1]);
        waitpid(child, NULL, 0);
        return;
    }
}

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
