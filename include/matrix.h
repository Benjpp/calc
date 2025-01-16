#ifndef MATRIX_H
#define MATRIX_H

/**
 * @brief: a matrix is a list of lists of integer type. We will keep a struct with 3 values:
 * the number of rows, the number of colums, and a pointer to the matrix itself. This header file serves only to define the structure,
 * as well as giving a mean of showing the contents of the matrix through standard output
 */
typedef struct{
    unsigned int rows;
    unsigned int columns;
    float **matrix;
}Matrix;

/**
 * @brief: creates a new (n)x(n) identity assigns it to a pointer. The necessary space is also allocated in this function.
 * 
 * @param n: size of the identity matrix
 * 
 * @param matrix_data: pointer to assign the new Matrix
 * 
 * @return: exit code
 */
int create_identity_matrix(unsigned int n, Matrix* matrix_data);

/**
 * @brief: creates a new (row)x(col) matrix initialized whith the numbers inputted
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
 * @param columns: number of columns the matrix has
 * 
 * @param matrix_data: pointer to assign the new matrix
 * 
 * @return: exit code
 */
int create_initialized_matrix(unsigned int rows, unsigned int columns, Matrix* matrix_data);

/**
 * @brief: prints to standard output the contents of a matrix in a organized manner
 * 
 * @param matrix: matrix to output
 */
void print_matrix(Matrix matrix_data);

/**
 * @brief: function used to free all resources allocated for a matrix
 * 
 * @param matrix_data: the matrix to be destroyed
 */
void destroy_matrix(Matrix* matrix_data);

/**
 * @brief: helper function to allocate memory for a matrix
 * 
 * @param rows, columns: dimensions of the matrix, thus dimensions of the array to allocate
 * 
 * @param matrix_ptr: a pointer to the matrix pointer
 * 
 * @return: exit code
 */
int matrix_allocator(unsigned int rows, unsigned int columns, float*** matrix_ptr);

#endif // MATRIX_H
