#include "token.h"
#include <stdio.h>

void print_token_in_list(token t) {
    write_in_token_buffer(t);
    printf("%s ", TOKEN_DISPLAY);
}

void print_token_list(token_array_list *list) {
    printf("{ ");
    apply_token_operation(list, print_token_in_list);
    printf("}\n");
}

void write_in_token_buffer(token t) {
    switch(t.type) {
        case INT :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[type=INT - value=%d]", t.int_val );
            break;
        case FLOAT :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[type=FLOAT - value=%f]", t.float_val );
            break;
        case DOUBLE :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[type=DOUBLE - value=%f]", t.double_val );
            break;
        case CHAR :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[type=CHAR - value=%f]", t.char_val );
            break;
        case IDENTIFIER:
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[type=IDENTIFIER - value=%s]", t.string_val );
            break;
        default:
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[type=CHAR - value=%f]", t.char_val );
            break;
    }

}