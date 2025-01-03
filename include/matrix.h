#ifndef MATRIX_H
#define MATRIX_H

/**
 * @brief: a matrix is a list of lists of integer type. We will keep a struct with 3 values:
 * the number of rows, the number of colums, and a pointer to the matrix itself. This header file serves only to define the structure,
 * as well as giving a mean of showing the contents of the matrix through standard output
 */
typedef struct{
    unsigned long rows;
    unsigned long colums;
    int **matrix;
}Matrix;

/**
 * @brief: creates a new 1x1 matrix initialized to a value of 0 and assigns it to a pointer
 * 
 * @param matrix_data: pointer to assign the new Matrix
 * 
 * @return: exit code
 */
int create_empty_matrix(Matrix* matrix_data);

/**
 * @brief: creates a new (row)x(col) matrix initialized whith the numbers passed as parameters.
 * The insertion of numbers will be as follows: say we have n colums in our matrix, then we will insert in each
 * row, starting from the first row ([0]) every n numbers we encounter while iterating through the given list. 
 * If there are more numbers passed in the list that space in the matrix the operation will be aborted, and if there 
 * are less numbers than total space, the resto will be 0
 * 
 * EXAMPLE:
 * Let us have the following list of numbers
 * int *numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
 * 
 * And a number of colums of 3, we would start by inserting in the fisrt row
 * 1, 2, 3
 * 
 * To the second row
 * 4, 5, 6
 * 
 * And to the third row
 * 7, 8, 9
 * 
 * @param rows: number of rows the matrix has
 * 
 * @param colums: number of colums the matrix has
 * 
 * @param numbers: list of numbers the matrix will be initialized to
 * 
 * @param number_elements: number of elements gived in the anterior list
 * 
 * @param matrix_data: pointer to assign the new matrix
 * 
 * @return: exit code
 */
int create_initialized_matrix(unsigned long rows, unsigned long colums, int numbers[], int number_elements, Matrix* matrix_data);

/**
 * @brief: prints to standard output the contents of a matrix in a organized manner
 * 
 * @param matrix: matrix to output
 */
void print_matrix(Matrix matrix_data);


#endif // MATRIX_H
