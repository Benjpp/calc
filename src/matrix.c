#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Not commenting this for now, cba too easy
int create_identity_matrix(unsigned int n, Matrix* matrix_data){
    float** matx;
    int exit_code;
    if((exit_code = matrix_allocator(n, n, &matx)) < 0){
        printf("Error %d. Propagated error in create_identity_matrix\n", exit_code);
    }
    for(unsigned int i = 0; i < n; i++){
        for(unsigned int j = 0; j < n; j++){
            if(i == j){
                matx[i][j] = 1;
                continue;
            }
            matx[i][j] = 0;
        }
    }

    matrix_data->rows = n;
    matrix_data->columns = n;
    matrix_data->matrix = matx;

    return 0;
}


int create_initialized_matrix(unsigned int rows, unsigned int columns, Matrix* matrix_data){
    float** matx;
    int exit_code;
    if((exit_code = matrix_allocator(rows, columns, &matx)) < 0){
        printf("Error %d. Propagated error in create_identity_matrix\n", exit_code);
    }

    for(unsigned int i = 0; i < rows; i++){
        for(unsigned int j = 0; j < columns; j++){
            printf("Enter END if u dont wish to enter more numbers. Otherwise, enter another number for row %d", i);    
            char inout_number[21];
            float aux;
            
        }
    }

    matrix_data->rows = rows;
    matrix_data->columns = rows;
    matrix_data->matrix = matx;

    return 0;
}

void print_matrix(Matrix matrix_data){
    for(unsigned int i = 0; i < matrix_data.rows; i++){
        for(unsigned int j = 0; j < matrix_data.columns; j++){
            printf("|%f| ", matrix_data.matrix[i][j]);
        }
        printf("\n");
    }
}

//Free to free resources (duhh)
void destroy_matrix(Matrix* matrix_data){
    for(unsigned long i = 0; i < matrix_data->columns; i++){
        free(matrix_data->matrix[i]);
    }

    free(matrix_data->matrix);
}

//Malloc to create space for matrices
int matrix_allocator(unsigned int rows, unsigned int columns, float*** matrix_ptr){
    float** matx = (float**)malloc(rows * sizeof(float));
    if(matx == NULL){
        perror("Error allocating space for matrix. Terminating process.\n");
        return -1;
    }

    for(unsigned int i = 0; i < rows; i++){
        matx[i] = (float*)malloc(columns * sizeof(float));
        if(matx[i] == NULL){
            perror("Error allocating space for matrix. Terminating process.\n");
            return-1;         
        }
    }

    (*matrix_ptr) = matx;

    return 0;
}