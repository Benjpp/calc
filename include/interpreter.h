#ifndef INTERPRETER_H
#define INTERPRETER_H

/**
 * @brief: definition of the interpreter for the mathematical expresions
 * Supported operations for now:
 * - Addition (+)
 * - Subtraction (-)
 */




typedef struct{
    char expresion_string[32];
    
    token info;
    node* left;
    node* right;
}node;

#define T_NUMBER 0
#define T_BINOP 1
#define T_BRACKET 2
typedef struct{
    int type;
    int value;
}token;

/**
 * @brief tokenizes the given expresion to parse through
 */

/**
 * @brief: creates a new tree that evaluates a given expresion 
 * 
 * @param expr: expresion to be evaluated
 * 
 * @return: pointer to the head of the tree
 */
node* evaluator(char* expr);        

/**
 * @brief: allocates space for a new node
 * 
 * @return: a pointer to the new node
 */
int allocator(node* n);

/**
 * @brief: evaluates if a given operator is supported
 * 
 * @param operator: operator to evaluate
 * 
 * @return: 0 if not supported, 1 for a unary operator, 2 for a binary operator
 */
int valid_operation(char operation[3]);

#endif INTERPRETER_H