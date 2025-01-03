#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>


int create_empty_matrix(Matrix* matrix_data){
    //reserve space for the only row
    int** matrix = (int**)malloc(1 * sizeof(int));
    
    //reserve space for the only colum
    matrix[0] = (int*)malloc(1 * sizeof(int));
    matrix[0][0] = 0;

    matrix_data->rows = 1;
    matrix_data->colums = 1;
    matrix_data->matrix = matrix;

    return 0;
}


int create_initialized_matrix(unsigned long rows, unsigned long colums, int* numbers, int number_elements, Matrix* matrix_data){
    if((rows * colums) < number_elements){
        return -1;
    }

    //Reserve space for rows
    int** matrix = (int**)malloc(rows * sizeof(int));

    //Looping through each row to reserve space  for colums
    for(unsigned long i = 0; i < colums; i++){
        matrix[i] = (int*)malloc(colums * sizeof(int));
    }

    //Initalizing matrix to given values
    for(unsigned long i = 0; i < rows; i++){
        for(unsigned long j = 0; j < colums; j++){
            if((i * j) <= number_elements){
                matrix[i][j] = numbers[i * colums + j];
                continue;
            }
            matrix[i][j] = 0;
        }
    }
    
    matrix_data->rows = rows;
    matrix_data->colums = colums;
    matrix_data->matrix = matrix;

    return 0;
}

void print_matrix(Matrix matrix_data){
    for(unsigned long i = 0; i < matrix_data.rows; i++){
        for(unsigned j = 0; j < matrix_data.colums; j++){
            printf("|%d| ", matrix_data.matrix[i][j]);
        }
        printf("\n");
    }
}
