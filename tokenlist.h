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

char std_peek(void); 
/* 
 * @function Peek next character in stdin
 * @return Next character in stdin
 */

char *ch2str(char c); 
/* 
 * @function Makes string out of char
 * @param c - character to make string
 * @return string consisting of one charachter
 */

char *get_word_std(void); 
/* 
 * @function Get characters before next space, new line or end of file
 * @return word from stdin
 */

int iscmd(char c); 
/* 
 * @function Checks if symbol is command symbol
 * @param c - character to check
 * @return 1 - is command symbol, 0 - not
 */

token_list *new_token(char *token); 
/* 
 * @function Creates new token with value (token, NULL)
 * @param *token - string to put in token
 * @return new token pointer
 */

void push_token(token_list **t, char *token); 
/* 
 * @function Add new token to list
 * @param **t - token list 
 * @param *token - token to put 
 */

void print_token_list(token_list *t); 
/* 
 * @function Prints list to stdout
 * @param **t - token list 
 */

void clear_token_list(token_list *t); 
/* 
 * @function Frees memory
 * @param **t - token list 
 */

token_list *get_token_list(void); 
/* 
 * @function Gets token list from stdin
 * @return token list from stdin
 */

#endif // TOKEN_LIST
