#ifndef TOKEN_LIST
#define TOKEN_LIST

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "config.h"

typedef struct token_list {
    char *token; /* Command, operation or operand */
    struct token_list *next; /* List pointer */
} token_list;

char std_peek(void); /* Next character in stdin */
char *ch2str(char c); /* Makes string out of char */
char *get_word_std(void); /* Get characters before next space, new line or end of file */
int iscmd(char c); /* Checks in symbol is command symbol */

token_list *new_token(char *token); /* Creates new token with value (token, NULL) */
void push_token(token_list **t, char *token); /* Add new token to list */
void print_token_list(token_list *t); /* Prints list to stdout */
void clear_token_list(token_list *t); /* Frees memory */

token_list *get_token_list(void); /* Gets token list from stdin */

#endif // TOKEN_LIST
