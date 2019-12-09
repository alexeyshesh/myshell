#include "exec.h"

void cmd_exec(token_list **t, int *status, int *conv_desc) {
    int child;
    if ((child = fork()) < 0) {
        fatal_error(PROC);
    } else if (child == 0) {
        if (isbgcmd(*t)) {
            int null_desc;
            if ((null_desc = open("/dev/null", O_RDONLY)) != -1) {
                dup2(null_desc, 0);
                close(null_desc);
                signal(SIGINT, SIG_IGN);
            } else {
                fprintf(stderr, "Error... so sad........\n");
                *status = -11;
                return;
            }
        } else {
            signal(SIGINT, SIG_DFL);
        }
        int exec_status;
        conv_exec(t, status, conv_desc, &exec_status);
        // log_msg(((*t)->token));
        if (strcmp(";", (*t)->token) == 0 || strcmp("&", (*t)->token) == 0) {
            (*t) = (*t)->next;
            exit(0);
        } else if (strcmp("||", (*t)->token) == 0) {
            if (!WIFEXITED(exec_status) || WEXITSTATUS(exec_status) != 0) {
                (*t) = (*t)->next;
                shell_cmd(t, status);
                exit(0);
            } else {
                next_cmd(t);
                shell_cmd(t, status);
                exit(0);
            }
        } else if (strcmp("&&", (*t)->token) == 0) {
            if (!WIFEXITED(exec_status) || WEXITSTATUS(exec_status) != 0) {
                next_cmd(t);
                shell_cmd(t, status);
                exit(0);
            } else {
                (*t) = (*t)->next;
                shell_cmd(t, status);
                exit(0);
            }
        }
    } else {
        int child_status;
        if (!isbgcmd(*t)) 
            waitpid(child, &child_status, 0);
        next_cmd(t);
        // if (*t)
        //     log_msg((*t)->token);
    }
}

void conv_exec(token_list **t, int *status, int *conv_desc, int *exec_status) {
    if (!(*t) || *status != 0) {
        *status = -9;
        return;
    }
    simple_cmd(t, status, conv_desc, exec_status);
    if ((*t)) {
        if (strcmp((*t)->token, "|") == 0) {
            (*t) = (*t)->next;
            conv_exec(t, status, conv_desc, exec_status);
        }
    }
}

void simple_cmd(token_list **t, int *status, int *conv_desc, int *exec_status) {
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

    simple_cmd_exec(t, command, argv, conv_desc, status, exec_status);

    free(argv);
}

void simple_cmd_exec(token_list **t, char *command, char **argv, int *conv_desc, int *status, int *exec_status) {

    if (strcmp("cd", command) == 0) {
        if (argv[1] == NULL)
            argv[1] = getenv("HOME");
        if (chdir(argv[1]) != 0) {
            fprintf(stderr, "%s: no such file or directory\n", argv[1]);
        }
        return;
    }

    int child;
    if (pipe(conv_desc) < 0) 
        fatal_error(PIPE_ERR);
    if ((child = fork()) < 0) {
        fatal_error(PROC);
    } else if (child == 0) {
        if (*t && strcmp((*t)->token, "|") == 0)
            dup2(conv_desc[1], 1);
        close(conv_desc[0]);
        close(conv_desc[1]);
        if (execvp(command, argv) == -1) {
            fprintf(stderr, "%s: command not found\n", command);
            *exec_status = -1;
        }
        exit(1);
    } else {
        dup2(conv_desc[0], 0);
        close(conv_desc[0]);
        close(conv_desc[1]);
        waitpid(child, exec_status, 0);
        return;
    }
}

void next_cmd(token_list **t) {
    int brackets = 0;
    while (*t) {
        if (strcmp((*t)->token, "(") == 0)
            brackets++;
        if (strcmp((*t)->token, ")") == 0)
            brackets--;
        
        if (brackets == 0 && strcmp((*t)->token, "&") == 0) {
            // (*t) = (*t)->next;
            return;
        }
        if ((brackets == 0 && strcmp((*t)->token, ";") == 0) || brackets < 0) {
            // (*t) = (*t)->next;
            return;
        }
        (*t) = (*t)->next;    
    }
    (*t) = new_token(";");
}