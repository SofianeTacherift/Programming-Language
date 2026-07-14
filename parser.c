#include "parser.h"
#include "token.h"


node * new_node() {
    node *res=malloc(sizeof(node));
    res->left=NULL;
    res->right=NULL;
    return res;
}


token *next(parser *parse ) {
    parse->current++;
}

token get_current_token(parser *parse) {
    if (parse->current<parse->tokens->size) {
        return parse->tokens->elements[parse->current];
    }
    return (token) {.type=END};
}



parser * new_parser(token_array_list * tokens) {
    void* p =malloc(sizeof(parser));
    parser *result = p;
    result->current=0;
    result->tokens=tokens;
    return result;
}


node * parse_identifier(parser * parse) {
    
}
node * parse_expression(parser *parse) {
    

}








