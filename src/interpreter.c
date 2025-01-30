#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Look up table for the operators, stored in a array
static const operation operator_table[] = {
	{"+", T_BINOP},
	{"-", T_BINOP},
    {"*", T_BINOP},
    {"END", 0}
};

//The number of characters in the longest operation will be kept in the following variable
int longest_operation = 5;
//A string with a message that the operator has not been found, used to be compared with the return array of next_token
char* operator_not_found = "Unsupported operation";

char* append_to_buffer(char* buffer, char to_append, int pos);
int lookup_optable(char* operation);
int get_variable_degree(char* expr, char* buffer, int pos);

/**
 * This functions works in combination with the char* next_token function, which is called to extracta token from where the pointer is currently 
 *poiting at
 */
token* tokenise(char* expr, int* number_of_tokens){
    token* token_list = malloc(sizeof(token));
    int pos = 0;
    //In case there are whitespaces i will use a pointer to integer (using pointer so i can access its value and modify it from outside this function scope) to account for it
    int* whitespaces = (int*)malloc(sizeof(int));
    int* is_number = (int*)malloc(sizeof(int));
    int* lookup_table_position = (int*)malloc(sizeof(int));
    while(*expr != '\0'){
        //Extraction of token, may be a number or a operation
        *whitespaces = 0;
        char* s_token = next_token(expr, whitespaces, is_number, lookup_table_position);
        if(s_token == operator_not_found){
            printf("Unsupported operation has been evaluated, exiting program. Please retry \n");
            return NULL;
        }
        
        printf("Token parsed: %s \n", s_token);
        //The token list is growed dynamically
        token_list = (token*)realloc(token_list, (sizeof(token) * (pos + 1)));
        token_list[pos] = create_token(s_token, is_number, lookup_table_position);
        pos++;
        //Have to move the pointer to the next token, which is in increment positions ahead
        expr += strlen(s_token) + (*whitespaces);
        free(s_token);
    }
    *number_of_tokens = pos;
    free(is_number);
    free(lookup_table_position);
    free(whitespaces);
    return token_list;
}

/**
 * Obtains the next token from the expresion te be parsed, it starts from where the pointer passed is at in the expresion being evaluated, another function
 * will be in charge of controlling what sections are being parsed and what position will be passed to this pointer
 */
char* next_token(char* expr, int* whitespaces, int* is_number, int* lookup_table_position){
	char* buffer = (char*)malloc(sizeof(char));
	int pos = 0;
    /**                         
     * I have separated in this snippet my if in two separate parts, if a number has been first detected, we shall expect for now a floating point number, and 
     * the next token will stop being generated as soon as me run into something that is not a number
     * TODO!! In another moment, if a number is firstly detected it might be a variable, wich can be interpreted as i did in my early calculus classes
     * In case the first character is not a number, we will append to the buffer this token and will stop when the operation is finally identified, in which case
     * we will have to begin another parse section to interpret the operation being performed if it is of unary type, as a sin could be. Ex: sqrt(2), ln(2)
     */

    //In case the expresion has whitespaces we skip them
    while((*expr) == ' '){
        expr++;
        (*whitespaces)++;
    }

	if((*expr >= '0' && *expr <= '9') || *expr == 'x'){
        //Must look for ^ that indicates the degree of the variable
		while((*expr >= '0' && *expr <= '9') || *expr == 'x'){
			buffer = append_to_buffer(buffer, *expr, pos);
			pos++;
            expr++;
		}
        //Now the degree must be extracted after the ^
        //If there is no ^ now, the degree is 1, otherwise parse the number afterwards
        if(*expr == '^'){
            buffer = append_to_buffer(buffer, *expr, pos);
            pos++;
            expr++;
            //We continue appending to the main token buffer plus we append the degree string to a new buffer to pass it into a integer
            int pos2 = 0;
            char* buffer2 = (char*)malloc(sizeof(char));
            while(*expr >= '0' && *expr <= '9'){
                buffer = append_to_buffer(buffer, *expr, pos);
                buffer2 = append_to_buffer(buffer2, *expr, pos2);
                pos2++;
			    pos++;
                expr++;
            }
            *is_number = atoi(buffer2) + 1;

        }else{
            *is_number = 1;
        }

        return buffer;
	}else{
        int counter = 0;
        int exit_code;
        //There might be case where the user might not use the tool correctly, so i will keep a global variable that has the length of the "longest operation",
        //As a way to exit the while loop in case a not supported operand is never found, and therefore we could be kept in a infinite loop
        if(*expr != '(' && *expr != ')'){
            do{
                //Dynamically allocating memory for the buffer
                buffer = append_to_buffer(buffer, *expr, pos);
                counter++;
                pos++;
                expr++;
            }while(counter <= longest_operation && (exit_code = valid_operation(buffer)) == -1);

            if(exit_code == -1){
                printf("Unsupported operator parsed \n");
                return operator_not_found;
            }
            *is_number = 0;
            *lookup_table_position = exit_code;
        }else{
            buffer = append_to_buffer(buffer, *expr, pos);
            *is_number = 0;
            //In case it is a token but not a operation, like a parenthesis, the pointer to the supposed position in the table will be made null and furthermore checked when creating a token
            lookup_table_position = NULL;
        }
        
        return buffer;
	}
}

/*
 * Auxiliar function to append a character to the end of a buffer
 * The same buffer passed as parameter is growed using realloc and is then returned
 * !TODO If the reallocation fails manage error
 */
char* append_to_buffer(char* buffer, char to_append, int pos){
    //printf("append_to_buffer -%s- -%c- -%d-\n\n", buffer, to_append, pos);
    char* new_buffer = (char*)malloc((sizeof(char)) * (pos + 1));
    if(new_buffer == NULL){
        printf("Error reallocating space for token buffer");
        return NULL;
    }
    strcpy(new_buffer, buffer);
    free(buffer);
    char* p = new_buffer + pos;
	sprintf(p, "%c", to_append);
    return new_buffer;
}
//Aux function that creates a new token struct and returns it, uses the info assigned by the next_token function
token create_token(char* s_token, int* is_number, int* lookup_table_position){
    token tok;
    if(*is_number > 0){
        tok.type = *is_number == 1? T_CONSTANT : T_VARIABLE;
        char* coefficient = (char*)malloc(sizeof(char));
        int pos = 0;
        while(*s_token != 'x' && *s_token != '\0'){
            coefficient = append_to_buffer(coefficient, *s_token, pos);
            pos++;
            s_token++;
        }
        //If pos is = 0, means the while directly encountered an x, which means the coefficient would be 1
        tok.value = malloc(sizeof(variable));
        ((variable*)(tok.value))->coefficient = pos == 0? 1 : atoi(coefficient);
        ((variable*)(tok.value))->degree = *is_number - 1;
    }else{
        //This number can either be 0 or greater, so in this block it would be 0, a operation
        //In this case we only store in our token struct the type it is according to the flags defined in the header file and we make the void pointer point to a integer value that holds the position
        //of the operation in the table
        tok.type = lookup_table_position == NULL? T_BRACKET : operator_table[*lookup_table_position].type;//If the pointer was made null by the next_token function it is a bracket type
        tok.value = malloc(sizeof(int));
        *(int*)(tok.value) = *lookup_table_position;
    }   


    return tok;
}

int valid_operation(char* operation){
	int exit_code = lookup_optable(operation);
    return exit_code;
}

//Iterates the array that contains the supported operations and looks for the given operation, returns -1 on error to control flow
int lookup_optable(char* operation){
	int it = 0;
	while(strcmp(operator_table[it].t_operation, "END") != 0 ){
		if(strcmp(operation, operator_table[it].t_operation) == 0){
			return it;
		}
        it++;     
	}
	return -1;
}

