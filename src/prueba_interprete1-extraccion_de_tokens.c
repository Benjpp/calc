#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>

//Auxiliar function for debugging, prints a token and its contents in a "nice" manner
void print_token(token tok);

int main(int argc, char* argv[]){
    int* number_of_tokens = (int*)malloc(sizeof(int));
    char* test_expresion1 = "3+    4x*5x^2";
    token* tokens = tokenise(test_expresion1, number_of_tokens);
    printf("Total list size -%d-\n", *number_of_tokens);
    printf("Total size of a single token struct -%ld-\n", sizeof(token));
    for(int i = 0; i < *number_of_tokens; i++)
        print_token(*(tokens++));
    free(number_of_tokens);
    

    return 0;
}

//Function to help debugging, prints a token in a nice manner
void print_token(token tok){
    switch(tok.type){
        case T_CONSTANT:
            printf("Constant: {%d, {%d,%d}}\n", tok.type, ((variable*)(tok.value))->coefficient, ((variable*)(tok.value))->degree);
            break;
        case T_VARIABLE:
            printf("Variable: {%d, {%d,%d}}\n", tok.type, ((variable*)(tok.value))->coefficient, ((variable*)(tok.value))->degree);
            break;
        case T_BINOP:
            printf("Binop: {%d, {%d}}\n", tok.type, *(int*)(tok.value));
            break;
        case T_UNOP:
            printf("Unop: {%d, {%d}}\n", tok.type, *(int*)(tok.value));
            break;
        case T_BRACKET:
            printf("Bracket/Parenthesis\n");
            break;
        default:
            printf("Error printing token");
            break;
    };
}

