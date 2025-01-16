#include "matrix.h"

/**
 * @brief: Tranformations operations, all will be handled as matrices where the order of vectors will be by rows
 * Vectors are 1xN matrices
 */

/**
 * @brief: Calculates the change of basis from the base m1 to the base m2, the basis are written as matrices where each vector
 * of the basis is placed by rows. 
 * 
 * @param m1: Original basis
 * 
 * @param m2: Destined basis
 * 
 * @param m: Vector/Coordinate to be transformed
 */
Matrix* basis_change(const Matrix m1, const Matrix m2, const Matrix m);