#include "tokenlist.h"

char std_peek() {
    char c = getchar();
    if (c != EOF)
        ungetc(c, stdin);
    return c;
}

char *ch2str(char c) {
    char *str = malloc(2);
    if (str == NULL)
        fatal_error(HEAP_OVERFLOW);
    sprintf(str, "%c", c);
    return str;
}

int iscmd(char c) {
    if (c == '&' || c == '|' || c == '>' || c == '<' || c == ';' || c == '(' || c == ')')
        return 1;
    else 
        return 0;
}

token_list *new_token(char *token) {
    token_list *t;
    if ((t = malloc(sizeof(token_list)))) {
        t->token = token;
      
        t->next = NULL;
        return t;
    } else {
        fatal_error(HEAP_OVERFLOW);
        return NULL;
    }
}

void push_token(token_list **t, char *token) {
    token_list *p = new_token(token);
    if (*t) {
        token_list *q = *t;
        while(q->next)
            q = q->next;
        q->next = p;
    } else
        *t = p;
}

void print_token_list(token_list *t) {
    printf("\nToken list:\n");
    while (t) {
        printf(t->next ? "├ " : "└ ");
        printf("`%s`\n", t->token);
        t = t->next;
    }
}

void clear_token_list(token_list *t) {
    if (t == NULL)
        return;
    if (t->next)
        clear_token_list(t->next);
    free(t->token);
    free(t);
}



token_list *get_token_list() {

    /* 
    * Possible operations in the order of their priority:
    * (expression)
    * | > >> < 
    * && ||
    * ; &
    */

    token_list *t = NULL;
    char c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (c == ' ')
            continue;
        if (!iscmd(c)) {
            ungetc(c, stdin);
            char *tmp = get_word_std();
            if (tmp == NULL) {
                /* Error: unclosed quote */
                clear_token_list(t);
                return NULL;
            }
            push_token(&t, tmp);
            continue;
        }
        if (c == '(' || c == ')' || c == '<' || c == ';') {
            push_token(&t, ch2str(c));
            continue;
        }
        if (c == '|' || c == '&' || c == '>') {
            if (c == std_peek()) {
                getchar();
                char *tmp = malloc(3);
                if (tmp == NULL)
                    fatal_error(HEAP_OVERFLOW);
                sprintf(tmp, "%c%c", c, c);
                push_token(&t, tmp);
            } else {
                push_token(&t, ch2str(c));
            }
            continue;
        }
        
        /* if we got here, some invalid character is used */
        printf("Error: unexpected symbol %c is used\n", c);
        clear_token_list(t);
        return NULL;
    }
    if (c == EOF && t == NULL) {
        printf("\nCommand file finished\n");
        exit(0);
    }
    return t;
}

char *get_word_std() {
    int size = 4, cur = 0;
    int q_flag = 0; /* quote flag */
    char c, *str;
    
    char *tmp = malloc(size * sizeof(char));
    if (tmp == NULL) {
        printf("Error: string is too long\n");
        return NULL;
    } else 
        str = tmp;
    while (((!iscmd(c = std_peek()) && c != ' ') || q_flag == 1) && c != '\n' && c != EOF) {
        c = getchar();
        if (c == '"') {
            q_flag ^= 1;
            continue;
        }
        if (cur == size - 2) {
            tmp = realloc(str, size *= 2);
            if (tmp == NULL) {
                printf("Error: string is too long\n");
                free(str);
                return NULL;
            } else 
                str = tmp;
        }
        str[cur++] = c;
    }

    if (q_flag != 0) {
        printf("Error: unclosed quote\n");
        return NULL;
    }

        
    if (cur == 0 && c == EOF)
        return NULL;
    str[cur] = 0;
    return str;
}
