#include "array_list.h"
#include <stdio.h>

#ifndef TOKEN_H
#define TOKEN_H

#define TOKEN_DISPLAY_SIZE() 100


static char * TOKEN_TYPE_NAMES[] = {
    "INT",
    "FLOAT",
    "DOUBLE",
    "CHAR",
    "AFFECTATION",
    "EQUALS",
    "NOT",
    "ADD",
    "SUB",
    "MULTIPLY",
    "DIVIDE",
    "DELIMITER",
    "PARENTHESE",
    "IDENTIFIER",
    "IF"
};

typedef enum {
    INT,
    FLOAT,
    DOUBLE,
    CHAR,
    AFFECTATION,
    EQUALS,
    NOT,
    ADD,
    SUB,
    MULTIPLY,
    DIVIDE,
    DELIMITER,
    PARENTHESE,
    IDENTIFIER,
    IF
} token_type;

typedef struct {
    token_type type;
    union {
    int int_val;
    float float_val;
    double double_val;
    char char_val;
    char *string_val;
    };
} token;


array_list(token);

static char TOKEN_DISPLAY[TOKEN_DISPLAY_SIZE()];


void print_token_in_list(token t);

void print_token_list(token_array_list *list);

void write_in_token_buffer(token t);


void reverse_number_token_value(token *t);








#endif