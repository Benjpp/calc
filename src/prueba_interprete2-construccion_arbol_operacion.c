#include "interpreter.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]){
  
    //Brackets testing

    char* test_expr = "(3*2+2)/(2+2)";
    int* counter = (int*)malloc(sizeof(int)); 
    token* list = tokenise(test_expr, counter);
    printf("Tokenising complete.\n");

    printf("List of tokens:\n");
    for(int i = 0; i < *counter; i++){
        print_token(list[i]);
    }
    bintree tree2 = construct_algebraic_operation_tree(list, *counter);
    display_bintree(tree2.root, 0);
    token res = evaluate_expr(tree2);
    printf("Resultado de %s = %f, expected 2\n", test_expr, ((variable*)(res.value))->coefficient);
     

    
    test_expr = "3+2-(5^3)*2";            //5-125*2=5-250=-245 
    list = tokenise(test_expr, counter);
    printf("Tokenising complete.\n");

    printf("List of tokens:\n");
    for(int i = 0; i < *counter; i++){
        print_token(list[i]);
    }
    tree2 = construct_algebraic_operation_tree(list, *counter);
    display_bintree(tree2.root, 0);
    res = evaluate_expr(tree2);
    int ires = 3+2- (pow(5,3) )*2;
    printf("Resultado de %s = %f, expected %d\n", test_expr, ((variable*)(res.value))->coefficient, ires);    
   
    test_expr = "2*(2+3*(2+1))-1";            //2*(2+3*3)-1=2*(2+9)-1=2*11-1=22-1 = 21
    list = tokenise(test_expr, counter);
    printf("Tokenising complete.\n");

    printf("List of tokens:\n");
    for(int i = 0; i < *counter; i++){
        print_token(list[i]);
    }
    tree2 = construct_algebraic_operation_tree(list, *counter);
    display_bintree(tree2.root, 0);
    res = evaluate_expr(tree2);
    printf("Resultado de %s = %f, expected 21\n", test_expr, ((variable*)(res.value))->coefficient);    
    
        
    test_expr = "3-2^(4-2)+1";            //3-2²+1=3-4+1=0
    list = tokenise(test_expr, counter);
    printf("Tokenising complete.\n");

    printf("List of tokens:\n");
    for(int i = 0; i < *counter; i++){
        print_token(list[i]);
    }
    tree2 = construct_algebraic_operation_tree(list, *counter);
    display_bintree(tree2.root, 0);
    res = evaluate_expr(tree2);
    printf("Resultado de %s = %f, expected 0\n", test_expr, ((variable*)(res.value))->coefficient);    
    
    test_expr = "2*(2+3*(2+1))-1";            //2*(2+3*3)-1=2*(2+9)-1=2*11-1=22-1 = 21
    list = tokenise(test_expr, counter);
    printf("Tokenising complete.\n");

    printf("List of tokens:\n");
    for(int i = 0; i < *counter; i++){
        print_token(list[i]);
    }
    tree2 = construct_algebraic_operation_tree(list, *counter);
    display_bintree(tree2.root, 0);
    res = evaluate_expr(tree2);
    printf("Resultado de %s = %f, expected 21\n", test_expr, ((variable*)(res.value))->coefficient);    
    
    test_expr = "3*2^2-12+3/2^0";
    ires = 3* pow(2,2) -12 + 3/pow(2,0);
    
    list = tokenise(test_expr, counter);
    printf("Tokenising complete.\n");

    printf("List of tokens:\n");
    for(int i = 0; i < *counter; i++){
        print_token(list[i]);
    }
    tree2 = construct_algebraic_operation_tree(list, *counter);
    display_bintree(tree2.root, 0);
    res = evaluate_expr(tree2);
    printf("Resultado de %s = %f, expected %d\n", test_expr, ((variable*)(res.value))->coefficient, ires);    
    

    
    return 0;
}
