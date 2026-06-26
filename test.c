#include "lexer.h"
#include <stdio.h>



int main(void) {
    char * code ="p=1; p=-p";
    token_array_list * list = lexe_code(code);
    print_token_list(list);
    free(list);
}