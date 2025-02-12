#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>

//Auxiliar function for debugging, prints a token and its contents in a "nice" manner
void print_token(token tok);

int main(int argc, char* argv[]){
    int* number_of_tokens = (int*)malloc(sizeof(int));
    char* test_expresion1 = "3+(34-23)/(4x*5x^2)";
    token* tokens = tokenise(test_expresion1, number_of_tokens);
    printf("Total list size -%d-\n", *number_of_tokens);
    printf("Total size of a single token struct -%ld-\n", sizeof(token));
    for(int i = 0; i < *number_of_tokens; i++)
        print_token(tokens[i]);
    free(number_of_tokens);
    

    return 0;
}

