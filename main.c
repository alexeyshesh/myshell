#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

#include "config.h"
#include "tokenlist.h"
#include "syntax.h"

int main(int argc, char **argv) {
    /* Now we use Git ! */
    signal(SIGINT, SIG_IGN);
    int in_desc = 0; /* input file descriptor */
    if (argc == 1) {
        in_desc = 0;
    } else if (argc == 2) {
        if ((in_desc = open(argv[1], O_RDONLY)) != -1) {
            dup2(in_desc, 0);
            close(in_desc);
        } else
            fatal_error(FILE_OPEN_ERROR);
    } else 
        fatal_error(INV_PAR);
    
    clear_screen();
    token_list *q = NULL;
    
    while (1) {
        if (in_desc == 0) {
            print_cwd();
            printf(": ");
        }
        
        q = get_token_list();
        if (q != NULL) {
            syntax_analysis(q);
            clear_token_list(q);
        }
    }

    while(wait(NULL) != -1);
    
    return 0;
}
