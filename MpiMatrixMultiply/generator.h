#ifndef MPIMATRIXMULTIPLY_GENERATOR_H
#define MPIMATRIXMULTIPLY_GENERATOR_H

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <mpi.h>

typedef unsigned long ul;
typedef unsigned long long ull;

void FreeMatrix(int** matrix, ull num_rows, ull num_cols) {
    for (int iter_free = 0; iter_free < num_rows; ++iter_free) {
        free(matrix[iter_free]);
    }
    free(matrix);
}

void AllocMatrixMemory(int** matrix, ul num_rows, ul num_cols) {
    matrix = (int**)malloc(num_rows * sizeof(int*));
    for (int iter_fill = 0; iter_fill < num_rows; ++iter_fill) {
        matrix[iter_fill] = (int*)malloc(num_cols * sizeof(int));
    }
}

void GenerateMatrix(int** matrix, ull num_rows, ull num_cols) {

}

#endif //MPIMATRIXMULTIPLY_GENERATOR_H
