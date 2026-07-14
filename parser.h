#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include <stdlib.h>

typedef struct {
    token_array_list *tokens;
    int current;

} parser;




typedef enum node_type {
    BINARY,
    AFFECTATION,
    INT,
    FLOAT,
    DOUBLE,
    VARIABLE,
    NUMBER,
    OPERATOR,


} node_type;

typedef enum operator {
    ADD,
    SUB,
    MULTIPLY,
    DIVIDE
} operator;

typedef union value {
    int_val;
    double_val;
    float_val;
    string_val;


} value;



typedef struct node {
    node_type *type;
    operator operation;
    union {
        int_val;
        float_val;
        double_val;
        string_val;
    };

    node *left;
    node *right;


} node;


parser * new_parser(token_array_list * token);

#endif