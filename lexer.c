
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"
#include "token.h"


bool is_new_token(char c) {
    switch (c)
    {
    case '*':
    case '+':
    case '/':
    case '-':
    case '=':
    case ' ':
        return true;
    default:
        return false;
    }
}

int lexe_number(token_array_list *tokens_list, char *code, int start, int str_end) {
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
    add_token(tokens_list, t);
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

token operator_to_token(char c) {
    switch (c) {
        case '+':
            return (token) {.type=ADD};
        case '-':
            return (token) {.type=SUB};
        case '*':
            return (token) {.type=MULTIPLY};
        case '/':
            return (token) {.type=DIVIDE};
        
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

bool is_operator_token(token t) {
    switch (t.type)
    {
    case ADD:
    case MULTIPLY :
    case SUB:
    case DIVIDE:
        return true;
    default:
        return false;
    }
}

bool is_multiply_minus(token previous_token) {
    return (is_operator_token(previous_token) || previous_token.type==AFFECTATION);
}



int lexe_minus(token_array_list* tokens_list, char *code, int i, int str_end) {
    int non_space_index=next_non_space_index(code, i+1, str_end);
    if (tokens_list->size==0 || is_multiply_minus(tokens_list->elements[tokens_list->size-1])) {
        if (non_space_index!=-1) {
            char next_char = code[non_space_index];
            if (isdigit(next_char)) {
  
                int res=lexe_number(tokens_list, code, non_space_index, str_end);
                token *new_number=tokens_list->elements+(tokens_list->size-1);
                reverse_number_token_value(new_number);
                return res;
            }
            else {
                
                add_token(tokens_list, (token) {.type=INT, .int_val=-1});
                add_token(tokens_list, (token) {.type=MULTIPLY});
                return i+1;
            }
        }
        else {
            // error
        }
    }
    add_token(tokens_list, (token) {.type=SUB});
    return i+1;
}
int lexe_operator(token_array_list* tokens_list, char *code, int i, int str_end) {
    if (i>=str_end) {return;}
    char operator=code[i];
    if (operator!='-') {
        add_token(tokens_list, operator_to_token(operator));
        return i+1;
    }

    else if (operator=='-') {
        return lexe_minus(tokens_list, code, i, str_end);
    }

}



token_array_list * lexe_code(char * code) {
    token_array_list *tokens_list = new_token_array_list();


    int i=0;
    char charI = code[i];
    int end=strlen(code);
    printf("end=%d\n", end);
    while (i<end) {
        charI=code[i];
        printf("i=%d - charI='%c' - list=", i, charI);
        print_token_list(tokens_list);
        if (isalpha(charI)) {
            i=lexe_string(tokens_list, code, i, end);
        }
        else if (isdigit(charI)) {
            i=lexe_number(tokens_list, code, i, end);
        }
        else if (is_operator(charI)) {
            i=lexe_operator(tokens_list, code, i, end);
        }
        else if (charI=='=') {
            add_token(tokens_list, ((token) {.type=AFFECTATION}));
            i++;
        }
        else {
            i++;
        }
    }

    return tokens_list;


}
