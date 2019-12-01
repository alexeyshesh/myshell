#ifndef CONFIG
#define CONFIG

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/wait.h>
#include <unistd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
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
    PIPE_ERR /* pipe() system call error */
};
typedef enum error_code error_code; 

void fatal_error(error_code code, ...); 
/* 
 * @function Exits program, probably with message
 * @param code - error code from error_code type
 * @param ... - possible to add message string
 */

void print_cwd(); 
/* 
 * @function Prints current working directory
 */

void log_msg(char *msg); 
/* 
 * @function prints log message to terminal
 * @param msg - log message
 */

void clear_screen(); 
/* 
 * @function clears screen
 */

#endif // CONFIG
