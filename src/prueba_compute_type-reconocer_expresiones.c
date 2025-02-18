#include "interpreter.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    char* function_like_expr = "f(x)=2x+1";
    expr_flag type = compute_expression_type(function_like_expr);

    printf("Computed type is: %u, expected 1\n", type);
    
    
    return 0;
}
