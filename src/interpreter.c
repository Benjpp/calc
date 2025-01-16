#include "interpreter.h"
#include <stdlib.h>
#include <string.h>


node* evaluator(char* expr){
    
}

/**HELP ME
 * @brief: both functions check if a father node has a son to its right/left
 * 
 * @param father: node on which to do the checking
 * 
 * @return: 0 if it doesnt have the son, 1 otherwise
 */
int left(node* father){
    if(father->left != NULL)
        return 1;
    return 0;
}

int right(node* father){
    if(father->right != NULL)
        return 1;
    return 0;
}


int allocator(node* n){
    n = (node*)malloc(sizeof(node));
    if(n == NULL){
        perror("Error. Couldnt allocate space for node.");
        return -1;
    }
    
    n->left = NULL;
    n->left = NULL;
    n->info = NULL;

    return 0;
}

int valid_operation(char operation[3]){
    if(strcmp(operation, "+") == 0){
        return 1;
    }else if(strcmp(operation, "-") == 0){
        return 1;
    }

    printf("Non supported operand %s passed through the program", operation);
    return 0;
}