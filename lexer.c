
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"


int lexe_number(token_array_list *tokens_list, char *code, int start, int str_end) {
    
}

int lexe_string(token_array_list* tokens_list, char *code, int start, int str_end) {
    int i=start;
    int current_len = 32;
    char * str =malloc(sizeof(char)*current_len);
    while (i<str_end && code[i] != ' ') {
        int current_index=i-start;
        char charI=code[i];

        printf("i=%d - current_index=%d - charI=%c\n", i, current_index, charI);
        if (current_index>current_len) {
            str=realloc(str, current_len*2*sizeof(char));
            current_len*=2;
        }
        str[current_index]=code[i];

        i++;

    }
    str[i-start]='\0';
    token token = {.string_val=str, .type=IDENTIFIER};
    add_token(tokens_list, token);

}



token_array_list * lexer(char * code) {
    token_array_list *tokens_list = new_token_array_list();

    int i=0;
    char charI = code[i];
    int end=strlen(code);
    while (i<end) {
        if (isalpha(charI)) {
            i=lexe_string(tokens_list, code, i, end);
        }

    }

}
