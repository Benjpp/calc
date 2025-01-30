#ifndef INTERPRETER_H
#define INTERPRETER_H
/**
 * @brief: definition of the interpreter for the mathematical expresions
 * Supported operations for now:
 * - Addition (+)
 * - Subtraction (-)
 */
#include "structures/bintree.h"

#define T_CONSTANT 0
#define T_VARIABLE 1
#define T_BINOP 2
#define T_UNOP 3
#define T_BRACKET 4

typedef struct token{
    int type;           //Flag to indicate what type of token. If it is T_Bracket, it means that a expresion in parenthesis is another tree pointed by this node pointer
    void* value;        //Pointer that can point either to a number/variable, or to a operation
}token;


typedef struct variable{
    int coefficient;    
    int degree;
}variable;              //A variable of degree = 0 is just a coefficient, which is just a constant number, otherwise it is just as any other variable, 
                        //with a coefficient and degree, !TODO posibility of adding a char to represent many variables in a expression

typedef struct operation{
	char* t_operation;
	int type;           //Flag to indicate if it is a unary or binary operation
                        //If the type is a unary operator, the number on wich to perform the operation goes into the left node,
                        //and the other number that represents the "order" of the operation into the right node (e.g, 2^3, 2 is the number to wich perform the "order" 3 of exponentiation)
}operation;


/**
 * @brief tokenizes the given expresion to parse through
 */

/**
 * @brief: extracts the tokens from the expresion
 * 
 * @param expr: expresion to be evaluated
 *
 * @param number_of_tokens: this value is modified to the number of tokens stored in the array
 *
 * @return: an array of tokens
 */
token* tokenise(char* expr, int* number_of_tokens);        

/**
 * @brief: returns the next token to be parsed in the expresion
 *
 * @param expr: the pointer from were which to start parsing a new token
 *
 * @param whitespaces: pointer to the number of whitespaces before the token, so that we can increment the pointer correctly for the next token
 * 
 * @param is_number: pointer to a flag type value, that is assigned to 1 if the token is a number, 0 if it is a operation, n if it is a variable in which
 * case the variable would be of degree n-1
 *
 * @param lookup_table_position: if is_number where of value 0, meaning the token to be returned is a operation, the value pointed at will be the position in the
 * table where the operation is
 */
char* next_token(char* expr, int* whitespaces, int* is_number, int* lookup_table_position);

/**
 * @brief: creates and allocates space for a new node for the binary tree
 *
 * @param token: token to be put into a node
 *
 * @param is_number, lookup_table_position: information about the token recollected in the anterior function
 */
token create_token(char* token, int* is_number, int* lookup_table_position);

/**
 * @brief: evaluates if a given operator is supported
 * 
 * @param operator: operator to parse
 * 
 * @return: the position in which the operator is located inside the table of supported
 * operations, -1 if nothing is found
 */
int valid_operation(char* operation);


#endif //INTERPRETER_H
