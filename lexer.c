
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"
#include "token.h"


#define NL_UPDATE_LEXER(LEXER, C) if ( C =='\n') {LEXER->current_line++; LEXER->current_char=0; }  
#define LEXER_ADV_CHAR(LEXER) LEXER->current_char++;

#define LEXER_ADV_UPDATE(LEXER , C) LEXER_ADV_CHAR(LEXER) NL_UPDATE_LEXER( LEXER, C)

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
    int line=lexe->current_line;
    int character=lexe->current_char;

    char * number =malloc(sizeof(char)*30);
    bool integer=true;
    while (i<str_end && !is_new_token(code[i]) &&isdigit(code[i])) {
        int current_index=i-start;
        char charI=code[i];


        if (charI=='.') {integer=false;} 

        number[current_index]=code[i];

        LEXER_ADV_UPDATE(lexe, charI)
        i++;

    }
    
    int end_index=i-start;
    number[end_index]='\0';
    token t;
    t.line=line;
    t.character=character;
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

int lexe_string(lexer *lexe, char *code, int start, int str_end) {
    int i=start;
    int current_len = 32;
    char * str =malloc(sizeof(char)*current_len);
    int line=lexe->current_line;
    int character=lexe->current_char;

    while (i<str_end && !is_new_token(code[i])) {
        int current_index=i-start;
        char charI=code[i];


        // printf("i=%d - current_index=%d - charI=%c\n", i, current_index, charI);
        if (current_index>current_len) {
            str=realloc(str, current_len*2*sizeof(char));
            current_len*=2;
        }
        str[current_index]=code[i];

        LEXER_ADV_UPDATE(lexe, charI)
        i++;

    }
    str[i-start]='\0';
    token t = {.string_val=str, .type=IDENTIFIER ,.line=line, .character=character};
    add_token(lexe->tokens_list, t);
    t.line=line;
    t.character=character;
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





int lexe_minus(lexer *lexe, char *code, int i, int str_end) {
    int non_space_index=next_non_space_index(code, i+1, str_end);
    int line=lexe->current_line;
    int character = lexe->current_char;
    LEXER_ADV_UPDATE(lexe, code[i]);

    token_array_list *tokens_list = lexe->tokens_list;

    if (tokens_list->size==0 || is_multiply_minus(tokens_list->elements[tokens_list->size-1])) {
        char next_char = code[non_space_index];
        if (isalpha(next_char) || isdigit(next_char) || next_char=='(' ){
            add_token(tokens_list, (token) {.type=UNARY_MINUS, .line=line, .character=character});
            return i+1;
        }        
    }
    add_token(tokens_list, (token) {.type=SUB, .line=line, .character=character});
    return i+1;
}
int lexe_operator(lexer *lexe, char *code, int i, int str_end) {
    char operator=code[i];
    int line=lexe->current_line;
    int character = lexe->current_char;
    LEXER_ADV_UPDATE(lexe, code[i]);
    if (operator!='-') {
        token result=operator_to_token(operator);
        result.character=character;
        result.line=line;
        add_token(lexe->tokens_list, result);
        return i+1;
    }

    else if (operator=='-') {
        return lexe_minus(lexe, code, i, str_end);
    }

}


int lexe_opening_parenthese(lexer *lexe, char *code, int i, int str_end) {
    add_token(lexe->tokens_list, (token) {.type=OPENING_PARENTHESE , .line=lexe->current_line, .character=lexe->current_char});
    LEXER_ADV_UPDATE(lexe, code[i]);
    return i+1;
}

int lexe_closing_parenthese(lexer *lexe, char *code, int i, int str_end) {
    add_token(lexe->tokens_list, (token) {.type=CLOSING_PARENTHESE, .character=lexe->current_char, .line=lexe->current_line});
    LEXER_ADV_UPDATE(lexe, code[i]);
    return i+1;
}



token_array_list * lexe_code(char * code) {
    lexer *lexe = new_lexer();

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
            result_i=lexe_string(lexe, code, i, end);
        }
        else if (isdigit(charI)) {
            result_i=lexe_number(lexe, code, i, end);
        }
        else if (is_operator(charI)) {
            result_i=lexe_operator(lexe, code, i, end);
        }
        else if (charI=='=') {
            add_token(lexe->tokens_list, ((token) {.type=AFFECTATION, .line=lexe->current_line, .character=lexe->current_char}));
            LEXER_ADV_UPDATE(lexe, charI)
            i++;
        }
        else if (charI==DELIMITATION) {

            add_token(lexe->tokens_list, (token) {.type=DELIMITER ,.line=lexe->current_line, .character=lexe->current_char  });
            LEXER_ADV_UPDATE(lexe, charI)
            i++;
        }
        else if (charI=='(') {
            result_i=lexe_opening_parenthese(lexe, code, i, end);
        }
        else if (charI==')') {
            result_i=lexe_closing_parenthese(lexe, code, i, end);
        }

        else {
            i++;
            LEXER_ADV_UPDATE(lexe, charI)
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
    add_token(lexe->tokens_list, (token) {.type=END, .line=lexe->current_line, .character=lexe->current_char});
    return lexe->tokens_list;


}
