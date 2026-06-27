#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include <stdlib.h>

typedef struct {
    token_array_list *tokens;
    int current;

} parser;


struct expression {
    
};
parser * new_parser(token_array_list * token);

#endif