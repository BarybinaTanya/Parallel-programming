#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define SIZER 1000
const long int N = 4000;

typedef unsigned long ul;
typedef unsigned long long ull;

int num_rows_diagram_p1 = 4;
int num_cols_diagram_p2 = 2;

void FreeMatrix(int** matrix, ull num_rows, ull num_cols) {
    for (int iter_free = 0; iter_free < num_rows; ++iter_free) {
        if (matrix[iter_free] == NULL) {
            perror("Memory error! Type 3");
            exit(-1);
        }
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

int randInt(int min, int max) {
    int range = (max - min);
    int div = RAND_MAX / range;
    return min + (rand() / div);
}

void GenerateMatrix(int** matrix, ull num_rows, ull num_cols) {
    if (matrix == NULL) {
        perror("Memory error! Type 0");
        exit(-1);
    }
    for (int i = 0; i < num_rows; ++i) {
        if (matrix[i] == NULL) {
            perror("Memory error! Type 1");
            exit(-1);
        }
        for (int j = 0; j < num_cols; ++j) {
            matrix[i][j] = randInt(-100, 100);
        }
    }
}

int** GetMatrixLocalLinesPtrByRank(int** matrix, int rank, ul local_A_rows, ull n2, int p1, int p2) {
    ul diagram_x = rank % p2;
    ul diagram_y = rank % p1;
    ul start_elem_matrix_A = SIZER * diagram_y * n2;
    ul num_elem_local_A = SIZER * n2;

    int** sendbuf;
//    for (ul iter_fill = start_elem_matrix_A; iter_fill < start_elem_matrix_A + num_elem_local_A; ++iter_fill) {
//
//    }
}

int main() {
    MPI_Init(NULL, NULL);
    int rank;
    int count_proc = num_cols_diagram_p2 * num_rows_diagram_p1;

    MPI_Comm_size(MPI_COMM_WORLD, &count_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//----------------------------------------------Counting-n1,-n2-and-n3--------------------------------------------------
    ull num_rows_matrix_A_n1, num_cols_matrix_A_rows_matrix_B_n2, num_cols_matrix_B_n3;
    num_rows_matrix_A_n1 = num_rows_diagram_p1 * SIZER;
    num_cols_matrix_A_rows_matrix_B_n2 = SIZER;
    num_cols_matrix_B_n3 = num_cols_diagram_p2 * SIZER;
//----------------------------------------Memory-allocate-and-generate-matrices-----------------------------------------

    ul local_A_rows = SIZER;
    int** local_matrix_A = NULL;
    AllocMatrixMemory(local_matrix_A, local_A_rows, num_cols_matrix_A_rows_matrix_B_n2);

    ul local_B_columns = SIZER;
    int** local_matrix_B = NULL;
    AllocMatrixMemory(local_matrix_B, num_cols_matrix_A_rows_matrix_B_n2, local_B_columns);

    if (rank == 0) {
        int** matrix_A = NULL;
        AllocMatrixMemory(matrix_A, num_rows_matrix_A_n1, num_cols_matrix_A_rows_matrix_B_n2);

        int** matrix_B = NULL;
        AllocMatrixMemory(matrix_B, num_cols_matrix_A_rows_matrix_B_n2, num_cols_matrix_B_n3);

        GenerateMatrix(matrix_A, num_rows_matrix_A_n1, num_cols_matrix_A_rows_matrix_B_n2);
        GenerateMatrix(matrix_B, num_cols_matrix_A_rows_matrix_B_n2, num_cols_matrix_B_n3);


    }
//----------------------------------------------------------------------------------------------------------------------

    FreeMatrix(local_matrix_A, SIZER, num_cols_matrix_A_rows_matrix_B_n2);
    return 0;
}
