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
    "IDENTIFIER"
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
    IDENTIFIER
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














#endif