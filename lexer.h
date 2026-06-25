#ifndef LEXER_H
#define LEXER_H

#include "token.h"






int lexe_string( token_array_list *list,  char *code, int start, int str_end);

token_array_list * lexe(char * code);


#endif