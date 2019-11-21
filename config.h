#ifndef CONFIG
#define CONFIG

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <unistd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

enum error_code {
    HEAP_OVERFLOW,
    FILE_OPEN_ERROR,
    NO_FILE, /* File doesn't exist */
    BAN_CH, /* Invalid character */
    INV_PAR, /* Invalid parametrs called */
    PROC, /* cannot fork */
    CMD_NOT_FOUND,
    CUR_DIR_ERR, /* Error getting current working directory */
    PIPE_ERR
};
typedef enum error_code error_code; 

void fatal_error(error_code code, ...); /* Exits program, probably with message */
void print_cwd(); /* Prints current working directory */
void clear_screen();

#endif // CONFIG