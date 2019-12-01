#include "config.h"

void fatal_error(error_code code, ...) {

    va_list msg; /* pointer to possible parameter */
    va_start(msg, code); /* places msg to parameter after code */ 

    printf(ANSI_COLOR_RED "\nFatal error: ");
    switch (code) {
        case HEAP_OVERFLOW: {
            printf("heap overflow\n");
        }
        break;
        case FILE_OPEN_ERROR: {
            printf("can not open file\n");
        }
        break;
        case NO_FILE: {
            printf("file doesn't exist\n");
        }
        break;
        case BAN_CH: {
            printf("forbidden character '%c' used\n", va_arg(msg, int));
        }
        break;
        case INV_PAR: {
            printf("invalid amount of parameters\n");
        }
        break;
        case PROC: {
            printf("can not start process\n");
        }
        break;
        case CMD_NOT_FOUND: {
            printf("command not found\n");
        }
        break;
        case CUR_DIR_ERR: {
            printf("can not get current directory address\n");
        }
        break;
        case PIPE_ERR: {
            printf("can not continue conveyor execution\n");
        }
        break;
    }
    printf(ANSI_COLOR_RESET);
    exit(1);
}

void print_cwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s", cwd);
    } else {
        fatal_error(CUR_DIR_ERR);
    }
}

void clear_screen() {
    int child;
    if ((child = fork()) < 0) {
        return;
    } else if (child == 0) {
        char *argv[2];
        argv[0] = "clear";
        argv[1] = NULL;
        execvp("clear", argv);
        exit(0);
    } else {
        waitpid(child, NULL, 0);
        return;
    }
}

void log_msg(char *msg) {
    printf(ANSI_COLOR_MAGENTA "\n%s" ANSI_COLOR_RESET "\n", msg);
}