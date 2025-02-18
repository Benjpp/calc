#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

//Look up table for the operators, stored in a array
static const operation operator_table[] = {
	{"+", T_BINOP, F_ADDITION},
	{"-", T_BINOP, F_SUBTRACTION},
    {"*", T_BINOP, F_MULTIPLICATION},
    {"/", T_BINOP, F_DIVISION},
    {"^", T_UNOP, F_EXPONENTIATION},
    {"END", 0}
};

//The number of characters in the longest operation will be kept in the following variable
int longest_operation = 5;
//A string with a message that the operator has not been found, used to be compared with the return array of next_token
char* operator_not_found = "Unsupported operation";

//Auxiliar functions for the ones declared in the header file, !TODO Slight documenting and overview of what they do
char* append_to_buffer(char* buffer, char to_append, int pos);
int lookup_optable(char* operation);
int get_variable_degree(char* expr, char* buffer, int pos);
void insert_constant(token* tok, bintree* tree);
void insert_variable(token* tok, bintree* tree);
int insert_binop(token* pre_token,token* operation, token* post_token, bintree* tree);
int insert_unop(token* pre_token, token* operation, token* post_token, bintree* tree);
node* search_tree(token* tok, node* root);
void parse_tree(node* n);
void evaluate_op(node* n);
int insert_brackets(token* bracket_tokens, bintree* tree, node* n);
token* bracket_tokens(token* parsed_tokens, int* number_tokens);

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

	if((*expr >= '0' && *expr <= '9') || *expr == 'x' || *expr == '.'){
        //Must look for ^ that indicates the degree of the variable
		while((*expr >= '0' && *expr <= '9') || *expr == 'x' || *expr == '.'){
			buffer = append_to_buffer(buffer, *expr, pos);
			pos++;
            expr++;
		}
        //Now the degree must be extracted after the ^
        //If there is no ^ now, the degree is 1, otherwise parse the number afterwards
        if(*expr == '^' && *(expr-1) == 'x'){
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
            *is_number = (atoi(buffer2) + 1);
            //free(buffer2); 
        }else if (*(expr-1) == 'x'){
            //If the last char is a x,(which is one pos back, because the while does a increment after completing a token, means it is a variable of degree 1,
            //which corresponds to n-1 = 1, n = 2
            *is_number = 2;
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
            //In case it is a token but not a operation, like a parenthesis, the pointer to the supposed position in the table will be made -2 and furthermore checked when creating a token
            //If it is (, the value is -2, if it is ), the value is -3. (This is not good practice i think)
            *lookup_table_position =  -2;
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
        ((variable*)(tok.value))->coefficient = pos == 0? 1 : strtod(coefficient, NULL);
        ((variable*)(tok.value))->degree = *is_number - 1;
        free(coefficient);
    }else{
        //This number can either be 0 or greater, so in this block it would be 0, a operation
        //In this case we only store in our token struct the type it is according to the flags defined in the header file and we make the void pointer point to a integer value that holds the position
        //of the operation in the table
        tok.type = *lookup_table_position == -2? /*second ternary expr, if it is -2 it is a parenthesis*/ (*s_token == '(' ? T_OBRACKET : T_CBRACKET) : operator_table[*lookup_table_position].type;
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

bintree construct_algebraic_operation_tree(token* token_list, int number_of_tokens){
    node* root =  create_node();
    int length;
    bintree tree = create_tree(root);
    for(int i = 0; i < number_of_tokens; i++){
        printf("Tree %d: type %d\n", i, token_list[i].type);
           switch(token_list[i].type){
            case T_CONSTANT:
                insert_constant(&token_list[i], &tree);
                break;
            case T_VARIABLE:
                insert_variable(&token_list[i], &tree);
                break;
            case T_BINOP:
                //A binop always has a precesor so no need to control array out of index, if it is the user has given an incorrect expresion
                //We use the length in case the post token is a bracket and we need to increment the i counter accordingly
                length = insert_binop(&token_list[i - 1], &token_list[i], &token_list[i + 1], &tree);
                i += length;
                break;
            case T_UNOP:
                length = insert_unop(&token_list[i - 1], &token_list[i], &token_list[i + 1], &tree);
                i += length;
                break;
            case T_OBRACKET:
                //When we encounter a parenthesis me must look for the closing one, so lets extract the tokens inside it
                //In the var length we return the amount of tokens in the bracket to increment the counter accordingly
                //PD: when inserting brackets, we pass the next token after the first bracket to the function call to insert the brackets, and the extraction  of tokens inside the bracket
                //stops when we encounter a bracket, so after we must increment the i counter to first go to the bracket, +1, and then it must go to the next token, +2
                length = insert_brackets(&token_list[i + 1], &tree, NULL);
                i += length + 1;
                break;
            case T_CBRACKET:
                
                break;
            default:
                printf("Error parsing token list in construct_operation_tree function");
                break;
        };
  
        display_bintree(tree.root, 0);
        printf("\n");
         
    }
    //For now 
    return tree;
}

//In both insert_constant and inseret_variable we must only look for the available son of the root, as the tree grows "up" or "down" as we append new operations
void insert_constant(token* tok, bintree* tree){
    if(tree->root->left == NULL){
        node* left_son = create_node();
        left_son->value = tok;
        insert_left(tree->root, left_son);
    }else{
        node* right_son = create_node();
        right_son->value = tok;
        insert_right(tree->root, right_son);
    }   
}

void insert_variable(token* tok, bintree* tree){
    if(tree->root->left == NULL){
        node* left_son = create_node();
        left_son->value = tok;
        insert_left(tree->root, left_son);
    }else{
        node* right_son = create_node();
        right_son->value = tok;
        insert_right(tree->root, right_son);
    }
}

int insert_binop(token* pre_token, token* operation, token* post_token, bintree* tree){
    int length = 1;
    //Manage a case in which the pre_token is a bracket, case where we would have to look where to append to, for that we will call another function that sets this scenario
    if((*(operator_table[*(int*)(operation->value)].t_operation) == '/') || (*(operator_table[*(int*)(operation->value)].t_operation) == '*')){        
        
        //In order to replace correctly the / and * operators we must grow the tree "down", so we search for the token that was before the binop string so that we can replace that node with the binary operation
        //and its corresponding arguments, which will be put from left to right just like in the expresion
        if(tree->root->value != NULL){
            node* aux = search_tree(pre_token, tree->root);
            //The value that preceded the operation will be substituted by the operation itself
            aux->value = operation;
            node* left_son = create_node();
            node* right_son = create_node();
            //The operation will have 2 sons which are its operators, and they will be inserted from left to right sons just as in the expression
            if(post_token->type == T_OBRACKET){
                length = insert_brackets(post_token + 1, tree, right_son) + 2;
                aux->right = right_son;
            }else{
                right_son->value = post_token;
                insert_right(aux, right_son);
            }
            left_son->value = pre_token;
            insert_left(aux, left_son);
        }else{
            //If the root has no value we just put the operation there
            tree->root->value = operation;
            node*post_token_node = create_node();
            if(post_token->type == T_OBRACKET){
                length = insert_brackets(post_token + 1, tree, post_token_node) + 2;
                tree->root->right = post_token_node;
            }else{
                post_token_node->value = post_token;
                insert_right(tree->root, post_token_node);  
            }
        }
    }else if((*(operator_table[*(int*)(operation->value)].t_operation) == '+') || (*(operator_table[*(int*)(operation->value)].t_operation) == '-')){
        //If it is either + or -, we just have to grow the tree "up", the operator is the new root, and to the left goes what was the old root, because we expect now another number still from
        //the list of tokens, that will occupy the right son
        if(tree->root->value != NULL){
            node* new_root = create_node();
            new_root->value = operation;
            new_root->left = tree->root;
            tree->root = new_root;
            node* post_token_node = create_node();
            //!TODO a function that inserts a bintree to the right or left of a node            
            if(post_token->type == T_OBRACKET){
                length = insert_brackets(post_token + 1, tree, post_token_node) + 2;
                tree->root->right = post_token_node;
            }else{
                post_token_node->value = post_token;
                insert_right(tree->root, post_token_node);  
            }   
        }else{

            tree->root->value = operation;
            node* post_token_node = create_node();
            if(post_token->type == T_OBRACKET){
                length = insert_brackets(post_token + 1, tree, post_token_node) + 2;
                tree->root->right = post_token_node;
            }else{
                post_token_node->value = post_token;
                insert_right(tree->root, post_token_node);  
            }
        }
    }
    return length;
}

//Its functioning is very similar to the insert_binop function
int insert_unop(token* pre_token, token* operation, token* post_token, bintree* tree){
    int length = 1;
    int pos = *(int*)(operation->value);
    operation_flags flag = operator_table[pos].op_flag;
    switch(flag){
        case F_EXPONENTIATION:
            if(tree->root->value != NULL){
                node* aux = search_tree(pre_token, tree->root);
                //The value that preceded the operation will be substituted by the operation itself
                aux->value = operation;
                node* left_son = create_node();
                node* right_son = create_node();
                //The operation will have 2 sons which are its operators, and they will be inserted from left to right sons just as in the expression
                if(post_token->type == T_OBRACKET){
                    length = insert_brackets(post_token + 1, tree, right_son) + 2;
                    aux->right = right_son;
                }else{
                    right_son->value = post_token;
                    insert_right(aux, right_son);
                }
                left_son->value = pre_token;
                insert_left(aux, left_son);
            }else{
                //If the root has no value we just put the operation there
                tree->root->value = operation;
                node*post_token_node = create_node();
                if(post_token->type == T_OBRACKET){
                    length = insert_brackets(post_token + 1, tree, post_token_node) + 2;
                    tree->root->right = post_token_node;
                }else{
                    post_token_node->value = post_token;
                    insert_right(tree->root, post_token_node);  
                }
            }
            break;
        default:
            printf("Error in insert_unop\n");
            break;
    };
    return length;
}

//This function should NOT return NULL, as the way this program work it is guranteed by its logic that the token* tok exists somewhere in the tree
//finds the node that has a equal value in the void pointer as the one passed as argument, in this case a token
node* search_tree(token* tok, node* root) {
    if (root == NULL) {
        return NULL;
    }
    token* aux = (token*)(root->value);
    if(aux == tok){
        return root;
    }

    node* found = search_tree(tok, root->left);
    if (found != NULL) {
        return found;
    }
    
    return search_tree(tok, root->right);
}

//Bracket extraction related functions
//
//Function used to get all tokens inside the brackets/parenthesis, we modify the integer value pointed at with the amount of tokens in the parent
//We have a bintree and a node parameter, if the node is null we just got called from the construct_operation_tree function, so we insert the result from where we would put into
//if it were a variable/constant if it is not null we got called from insert_binop
//which means that we have to insert the result to node* n,
int insert_brackets(token* bracket_tok, bintree* tree, node* n){

    //First we determine into what  node we must insert the expresion in the brakcets
    int called_from_binop = n == NULL ? 0 : 1;
    //If we werent called from binop, we will append the bracket to the first available left or right sons from the root node
    //We will use the same null node passed to do this
    if(called_from_binop == 0){
        n = create_node();
        //I will store in integer passed as argument a value of 1 or 0 in its value, 1 if it is put into the right, 0 if it was inserted in the left(In the case we were called from the main
        //construction function)
        if(tree->root->left == NULL){
            tree->root->left = n;
        }else{
            tree->root->right = n;
        }
    }
    //Now use  the auxiliar function to get the tokens inside the brackets
    int* number_tokens = (int*)malloc(sizeof(int));
    *number_tokens = 0;
    token* inside_brackets = bracket_tokens(bracket_tok, number_tokens);

    //Now we solve the bracket
    bintree bracket_sub_tree = construct_algebraic_operation_tree(inside_brackets, *number_tokens);
    //We put the result into the position occupied byu the closing bracket
    token* bracket_result = bracket_tok + *number_tokens;
    *bracket_result = evaluate_expr(bracket_sub_tree);
    //insert_bracket can also be called from inside a bracket, in which case we do not need to apend to any tree, because the goal is to append the resulting token to the list of tokens from the
    //original bracket, so if both tree and n are null, we dont want to insert jackshit
    n->value = bracket_result;
    //We must put into the closing bracket the result pointer now
   
    free(inside_brackets);
    printf("IN BRACKS\n\n");
    return (*number_tokens);                                                                  
}

//Return the tokens that are inside the parenthesis, parsed_tokens already points at the first ocurrence after the first opening parenthesis
token* bracket_tokens(token* parsed_tokens, int* number_tokens){
    printf("bracket_tokens\n");
    token* brackets = (token*)malloc(sizeof(token));
    int counter = 0;
    //A parenthesis/bracket is closed using ), so we check for it in case it is another parenthesis inside a parenthesis
    //Reminder, value -2 for (, -3 for )
    int C_brackets = 1;
    while(C_brackets > 0){
        brackets = (token*)realloc(brackets, sizeof(token) * (counter+1));
        if(parsed_tokens->type == T_OBRACKET){
            C_brackets++;
        }
        //If we encounter a closing bracket and C_bracket value is 1, it means we must stop on this iteration
        if(parsed_tokens->type == T_CBRACKET){
            C_brackets--;
        }
        print_token(*parsed_tokens);
        brackets[counter] = *parsed_tokens;
        counter++;
        parsed_tokens++;
        
        }   
    //Because in the last iteration we append the last closing bracket, which is already accounted for in the increments in the calls from insert_binop and construct algebriac_tree we 
    //subtract 1 from the total count of tokens
    *number_tokens = counter - 1;
    return brackets;
}

//Function to help debugging, prints a token in a nice manner
void print_token(token tok){
    switch(tok.type){
        case T_CONSTANT:
            printf("Constant: {%d, {%f,%f}}\n", tok.type, ((variable*)(tok.value))->coefficient, ((variable*)(tok.value))->degree);
            break;
        case T_VARIABLE:
            printf("Variable: {%d, {%f,%f}}\n", tok.type, ((variable*)(tok.value))->coefficient, ((variable*)(tok.value))->degree);
            break;
        case T_BINOP:
            printf("Binop: {%d, {%d}}\n", tok.type, *(int*)(tok.value));
            break;
        case T_UNOP:
            printf("Unop: {%d, {%d}}\n", tok.type, *(int*)(tok.value));
            break;
        case T_OBRACKET:
            printf("OBracket/Parenthesis. TYPE: %d\n", tok.type);
            break;
        case T_CBRACKET:
            printf("CBracket/Parenthesis. TYPE: %d\n", tok.type);
            break;
        default:
            printf("Error printing token\n");
            break;
    };
}

//Wrapper function used to setup the recursive parsing
//The result is stored in a final token
token evaluate_expr(bintree op_tree){
    
    token* result = (token*)malloc(sizeof(token));
    node* root = op_tree.root;
    result->value = malloc(sizeof(variable));
    ((variable*)(result->value))->coefficient = 0;
    parse_tree(root);
    return *(token*)(root->value);
}

void print_node_coefficient(node* n){
    if(((token*)(n->value))->type == T_CONSTANT || ((token*)(n->value))->type == T_VARIABLE){
        printf("Node with coefficient %f\n", ((variable*)(((token*)(n->value))->value))->coefficient);
    }
}

//Function used to traverse the operation tree, starts in the root node and uses a post order traversal to evaluate the tree
//When we eval a opeartion, we change the node and turn it into a constant
void parse_tree(node* n){
    if(n == NULL){
        return;
    }
               
    parse_tree(n->left);
    parse_tree(n->right);

    switch(((token*)(n->value))->type){
        case T_CONSTANT:
            
            break;
        case T_VARIABLE:

            break;
        case T_BINOP:
            evaluate_op(n);
            print_node_coefficient(n);
            break;
        case T_UNOP:
            evaluate_op(n);
            print_node_coefficient(n);
            break;
        default:
            printf("Error parsing the operation tree");
            break;
    };
}

//Auxiliar function used to calculate the result of a binary operation in the tree, and acumulates the sum in the value pointed by result
void evaluate_op(node* n){
    int pos = *(int*)(((token*)(n->value))->value);
    double op1, op2, res;
    token* modified_token =(token*)(n->value);
    switch(operator_table[pos].op_flag){
        case F_ADDITION:
            
            op1 = ((variable*)(((token*)(n->left->value))->value))->coefficient;
            op2 = ((variable*)(((token*)(n->right->value))->value))->coefficient;
            res = op1+op2;
            modified_token->value = (variable*)malloc(sizeof(variable));
            modified_token->type = T_CONSTANT;
            ((variable*)(modified_token->value))->coefficient = res;
            break;
        case F_SUBTRACTION:
            op1 = ((variable*)(((token*)(n->left->value))->value))->coefficient;
            op2 = ((variable*)(((token*)(n->right->value))->value))->coefficient;
            res = op1-op2;
            modified_token->value = (variable*)malloc(sizeof(variable));
            modified_token->type = T_CONSTANT;
            ((variable*)(modified_token->value))->coefficient = res;
            break;
        case F_MULTIPLICATION:
            op1 = ((variable*)(((token*)(n->left->value))->value))->coefficient;
            op2 = ((variable*)(((token*)(n->right->value))->value))->coefficient; 
            res = op1*op2;
            modified_token->value = (variable*)malloc(sizeof(variable));
            modified_token->type = T_CONSTANT;
            ((variable*)(modified_token->value))->coefficient = res;
            break;
        case F_DIVISION:
            op1 = ((variable*)(((token*)(n->left->value))->value))->coefficient;
            op2 = ((variable*)(((token*)(n->right->value))->value))->coefficient;
            res = op1/op2;
            modified_token->value = (variable*)malloc(sizeof(variable));
            modified_token->type = T_CONSTANT;
            ((variable*)(modified_token->value))->coefficient = res;
            break;
        case F_EXPONENTIATION:
            op1 = ((variable*)(((token*)(n->left->value))->value))->coefficient;
            op2 = ((variable*)(((token*)(n->right->value))->value))->coefficient;
            res = pow(op1,op2);
            modified_token->value = (variable*)malloc(sizeof(variable));
            modified_token->type = T_CONSTANT;
            ((variable*)(modified_token->value))->coefficient = res;
            break;
        default:
            printf("Error operating\n");
            break;
    };
}

/*Look up table for the operators, stored in a array
static const operation operator_table[] = {
	{"+", T_BINOP, F_ADDITION},
	{"-", T_BINOP, F_SUBTRACTION},
    {"*", T_BINOP, F_MULTIPLICATION},
    {"/", T_BINOP, F_DIVISION},
    {"^", T_UNOP, F_EXPONENTIATION},
    {"END", 0}
};
*/

//Auxiliar function that parses the variables inside a parenthesis of a function expresion and return how many there are
//Approach: when we write down a function, in the parenthesis the variables are separated by comas, and because we exclusively need to count the number of variables, we can just count the number of
//comas and add +1, due to for ex f(x) has 0 comas 1 var, f(x,y) has 1 coma 2 vars,...
int count_variables(char* expr){
    int var_count = 0;
    while(*expr != ')'){
        if(*expr == ','){
            var_count++;
        }
        expr++;
    }
    return var_count + 1;
}

expr_flag compute_expression_type(char* expr){
    expr_flag type;
    if((*expr >= 'A' && *expr <= 'Z') || (*expr >= 'a' && *expr <= 'z')){
        //If the first char is a simple letter we are expecting maybe more letters until we reach a opening parenthesis, we could also be expecting a variable
        expr++;
        //We pass the string part where there are the first letters
        while((*expr >= 'A' && *expr <= 'Z') || (*expr >= 'a' && *expr <= 'z')){
            expr++;
        }
        //First encountered letters from a variable passed
        if(*expr == '('){
            //Functions start with a letter followed by a opening parenthesis, which means that after passing the first starting letters, if there is a parenthesis we must conclude it is a function
            //Now we must compute how many variables are in the function
            expr++;
            int variable_count = count_variables(expr);
            switch (variable_count){
                case 1:
                    type = EX_FUNCTION1;
                    break;

                default:
                    printf("Error computing variable count in inputted function\n");
                    break;
            };
        }
    }
    return type;
}

