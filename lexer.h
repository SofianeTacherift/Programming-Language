#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdio.h>


#define DELIMITATION ';'
#define NOT_USED_RES_I -2
#define ERROR_RES_I -1


static char ERROR_BUFFER[1024];



int lexe_number(token_array_list *tokens_list, char *code, int start, int str_end);

int lexe_string( token_array_list *list,  char *code, int start, int str_end);

bool is_new_token(char c);

bool is_operator(char c);

token operator_to_token(char c);


token_array_list * lexe_code(char * code);


#endif