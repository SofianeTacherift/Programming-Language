#include "parser.h"
#include "token.h"

#define P_NEW_LINE printf("\n");

#define RET_NULL_IF_ERROR if (parse->parsing_status== PARSING_ERROR ) { return NULL; }



node * new_node() {
    node *res=malloc(sizeof(node));
    res->left=NULL;
    res->right=NULL;
    return res;
}


token get_current_token(parser *parse) {
    if (parse->current<parse->tokens->size) {
        return parse->tokens->elements[parse->current];
    }
    return (token) {.type=END};
}

token get_next_token(parser *parse) {
    if (parse->current+1<parse->tokens->size) {
        return parse->tokens->elements[parse->current+1];
    }
    return (token) {.type=END}; 
}

token advance(parser *parse ) {
    parse->current++;
    return get_current_token(parse);
}

bool parsing_error(parser *parse) {
    return parse->parsing_status==PARSING_ERROR;
}



parser * new_parser(token_array_list * tokens) {
    void* p =malloc(sizeof(parser));
    parser *result = p;
    result->current=0;
    result->tokens=tokens;
    return result;
}


node * parse_statement(parser *parse) {
    token current = get_current_token(parse);
    if (current.type=IDENTIFIER) {
        node * result= (get_next_token(parse).type==AFFECTATION) ? parse_affectation(parse) : parse_expression(parse);
        return result;
    }
    else {    
        return parse_expression(parse);
    }
}


node * parse_affectation(parser *parse) {
        
    RET_NULL_IF_ERROR

    node *left = parse_identifier(parse);
    node * result = new_node();
    advance(parse);
    node * right = parse_expression(parse);
    result->type=AFFECTATION_NODE;
    result->left=left;
    result->right=right;
    return result;

}

node *parse_expression(parser *parse) {

    return parse_additive(parse);
}



node * parse_identifier(parser * parse) {
    node * res = new_node();
    token current = get_current_token(parse);
    res->type=VARIABLE_NODE;
    res->string_val=current.string_val;
    advance(parse);
    return res;
}






node * token_num_to_node(token t) {
    node * node=new_node();
    switch (t.type) {
    case INT:
        node->type=INT_NODE;
        node->int_val=t.int_val;
        break;
    case FLOAT:
        node->type=FLOAT_NODE;
        node->int_val=t.float_val;
        break;
    case DOUBLE:
        node->type=DOUBLE_NODE;
        node->int_val=t.double_val;
        break;
    default:
        return NULL;
    return node;
    }
}


bool is_unary_operator_token(token t ) {
    return t.type==UNARY_MINUS;
}

node * unary_token_to_node(token t) {
    node * result = new_node();
    if (t.type=UNARY_MINUS) {
        result->type=UNARY_NODE;
        result->operation=UNARY_MINUS;
    }
    else {
        //error
    }
    return result;
}

node * parse_unary(parser * parse) {
    token t = get_current_token(parse);
    node * res = unary_token_to_node(t);
    advance(parse);
    res->right=parse_primary(parse);
    return res;
}
node * parse_primary(parser * parse) {
    token current = get_current_token(parse);
    node * result;
    if (is_num_token(current)) {
        result=token_num_to_node(current);
        advance(parse);
    }
    else if (is_unary_operator_token(current)) {
        result = parse_unary(parse);
    }
    else if (current.type==IDENTIFIER) {
        result=parse_identifier(parse);
    }
    else if (current.type==OPENING_PARENTHESE) {
        advance(parse);
        token t= get_current_token(parse);

        result=parse_expression(parse);

        advance(parse);
    }
    else {
        parse->parsing_status=PARSING_ERROR;
        return NULL;
    }


    return result;
}


node * parse_multiplicative(parser * parse) {
    node *left = parse_primary(parse);

    node *center=NULL;
    token current;

    while ((current=get_current_token(parse)).type==MULTIPLY || current.type==DIVIDE) {
        node *new_center= new_node();
        new_center->type=BINARY_NODE;

        new_center->operation=(current.type==MULTIPLY) ? MULTIPLY_OPERATOR : DIVIDE_OPERATOR;

        if (center==NULL) {
            new_center->left=left;
        }
        else {
            new_center->left=center;
        }
        advance(parse);
        new_center->right=parse_primary(parse);
        center=new_center;
    }
    // printf("center=");
    // display_node(center->right);
    // printf("\n");
    if (center==NULL) {center = left;}
    return center;
}



node * parse_additive(parser *parse) {
    node *left=parse_multiplicative(parse);
    node *center=NULL;
    token current;
    while ((current = get_current_token(parse)   ).type==ADD || current.type==SUB ) {

        node * new_center =new_node();
        new_center->type=BINARY_NODE;

        new_center->operation=(current.type==ADD) ? ADD_OPERATOR : SUB_OPERATOR;
        if (center==NULL) {
            new_center->left=left;
        }
        else {
            new_center->left=center;
        }
        advance(parse);
        new_center->right=parse_multiplicative(parse);
        center=new_center;
    }
    if (center==NULL) {center = left;}

    return center;


}






void print_operation(operator op) {
    switch (op) {
    case ADD_OPERATOR:
        printf("+");
        break;
    case SUB_OPERATOR:
        printf("-");
        break;
    case MULTIPLY_OPERATOR:
        printf("*");
        break;
    case DIVIDE_OPERATOR:
        printf("/");
        break;
    case UNARY_MINUS:
        printf("unary -");
        break;
    }
}

void print_num_val(node *n) {
    switch(n->type) {
        case INT_NODE:
            printf("%d", n->int_val);
            break;
        case FLOAT_NODE:
            printf("%f", n->float_val);
            break;
        case DOUBLE_NODE:
            printf("%lf", n->double_val);
            break;
    }
}

bool is_num_node(node * n) {
    return n->type==INT_NODE || n->type==FLOAT_NODE || n->type==DOUBLE_NODE;
}
void display_node(node * n) {
    if (n==NULL) {printf("NULL");}
    printf("node[ type= %s ", NODE_TYPE_STR[n->type]);
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf("operation = ");
        print_operation(n->operation);
        printf(" ");
    }
    if (n->type==VARIABLE_NODE) {
        printf("name= %s ", n->string_val);
    }
    if (is_num_node(n)) {
        printf("value=");
        print_num_val(n);
    }
    printf(" ]");
}

void display_tree_node(node * n) {
    if (n==NULL) {return;}
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf("( ");
    }
    display_tree_node(n->left);
    display_node(n);
    printf(" ");
    display_tree_node(n->right);
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf(") ");
    }
}












