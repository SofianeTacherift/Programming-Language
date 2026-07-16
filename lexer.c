
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"
#include "token.h"


#define NL_UPDATE_LEXER(C, LEXER) if (C=='\n') {LEXER->current_line++; LEXER->current_char=0; }  
#define LEXER_ADV_CHAR(LEXER) LEXER->current


// return true if a substring who start with c is automaticly a new token


lexer * new_lexer() {
    lexer *res = malloc(sizeof(lexer));
    if (res!=NULL) {
        res->current_char=0;
        res->current_line=0;
        res->tokens_list=new_token_array_list();
        res->lexing_status=NOT_USED_RES_I;
    }
    return res;
}

bool is_new_token(char c) {
    switch (c)
    {
    case '*':
    case '+':
    case '/':
    case '-':
    case '=':
    case ' ':
    case '(':
    case ')':
    case DELIMITATION:
        return true;
    default:
        return false;
    }
}

int lexe_number(lexer *lexe, char *code, int start, int str_end) {
    int i=start;
    char * number =malloc(sizeof(char)*30);
    bool integer=true;
    while (i<str_end && !is_new_token(code[i])) {
        int current_index=i-start;
        char charI=code[i];

        if (charI=='.') {integer=false;} 

        number[current_index]=code[i];

        i++;

    }
    
    int end_index=i-start;
    number[end_index]='\0';
    token t;
    if (integer) {
        t.type=INT;
        t.int_val=atoi(number);
    }
    else {
        bool is_double=(end_index>0 && number[end_index-1]=='f') ? false : true;
        t.type=(is_double) ? DOUBLE : FLOAT;
        if (is_double) {
            t.double_val=atof(number);
        }
        else {
            t.float_val=atof(number);
        }
    }
    add_token(lexe->tokens_list, t);
    return i;

}

int lexe_string(token_array_list* tokens_list, char *code, int start, int str_end) {
    int i=start;
    int current_len = 32;
    char * str =malloc(sizeof(char)*current_len);
    while (i<str_end && !is_new_token(code[i])) {
        int current_index=i-start;
        char charI=code[i];

        // printf("i=%d - current_index=%d - charI=%c\n", i, current_index, charI);
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
    return i;
}

bool is_operator(char c) {
    switch (c) {
        case '+':
        case '-':
        case '*':
        case '/':
            return true;
        default:
            return false;
    }
}



char next_non_space_index(char * str, int i, int end) {
    for (int j=i; j<end;j++) {
        if (str[j]!=' ') {
            return j;
        }
    }
    return -1;
}





int lexe_minus(token_array_list* tokens_list, char *code, int i, int str_end) {
    int non_space_index=next_non_space_index(code, i+1, str_end);
    if (non_space_index==-1) {
        return ERROR_RES_I;
    }
    if (tokens_list->size==0 || is_multiply_minus(tokens_list->elements[tokens_list->size-1])) {
        char next_char = code[non_space_index];
        if (isalpha(next_char) || isdigit(next_char) || next_char=='(' ){
            add_token(tokens_list, (token) {.type=UNARY_MINUS});
            return i+1;
        }        
    }
    add_token(tokens_list, (token) {.type=SUB});
    return i+1;
}
int lexe_operator(token_array_list* tokens_list, char *code, int i, int str_end) {
    char operator=code[i];
    if (operator!='-') {
        add_token(tokens_list, operator_to_token(operator));
        return i+1;
    }

    else if (operator=='-') {
        return lexe_minus(tokens_list, code, i, str_end);
    }

}


int lexe_opening_parenthese(token_array_list* tokens_list, char *code, int i, int str_end) {
    add_token(tokens_list, (token) {.type=OPENING_PARENTHESE});
    return i+1;
}

int lexe_closing_parenthese(token_array_list* tokens_list, char *code, int i, int str_end) {
    add_token(tokens_list, (token) {.type=CLOSING_PARENTHESE});
    return i+1;
}



token_array_list * lexe_code(char * code) {
    token_array_list *tokens_list = new_token_array_list();


    int i=0;
    int result_i=NOT_USED_RES_I;
    char charI = code[i];
    int end=strlen(code);
    // printf("end=%d\n", end);
    while (i<end) {
        charI=code[i];
        // printf("i=%d - charI='%c' - list=\n", i, charI);
        // print_token_list(tokens_list);
        if (isalpha(charI)) {
            result_i=lexe_string(tokens_list, code, i, end);
        }
        else if (isdigit(charI)) {
            result_i=lexe_number(tokens_list, code, i, end);
        }
        else if (is_operator(charI)) {
            result_i=lexe_operator(tokens_list, code, i, end);
        }
        else if (charI=='=') {
            add_token(tokens_list, ((token) {.type=AFFECTATION}));
            i++;
        }
        else if (charI==DELIMITATION) {

            add_token(tokens_list, (token) {.type=DELIMITER });
            i++;
        }
        else if (charI=='(') {
            result_i=lexe_opening_parenthese(tokens_list, code, i, end);
        }
        else if (charI==')') {
            result_i=lexe_closing_parenthese(tokens_list, code, i, end);
        }

        else {
            i++;
        }

        if (result_i==ERROR_RES_I) {
            snprintf(ERROR_BUFFER, 1024, "Error during lexing after character %d (%c)\n",i+1, code[i] );
            fwrite(ERROR_BUFFER, sizeof(char), 1024, stderr);
            return NULL;
        }
        if (result_i!=NOT_USED_RES_I) {
            i=result_i;
            result_i=NOT_USED_RES_I;
        }
        // printf("affected i=%d\n", i);

    }
    printf("\ncode lexed\n");
    add_token(tokens_list, (token) {.type=END});
    return tokens_list;


}
