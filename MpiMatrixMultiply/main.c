#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <mpi.h>

#include "generator.h"

#define SIZER 1000

//ul GetProcessFromDiagram(ul row, ul col, ul num_rows_diagram, ul num_cols_diagram) {
//    return row * num_cols_diagram + col;
//}

ul GetNumberLocalMatrixRowsOrCols() {
    return SIZER;
    // Total number of rows at matrix_A is n1. Find number of elements at each
    // process: (n1) / p1 = (p1 * SIZER) / p1 = SIZER. Same with (n2 * n3) / p2.
}
//

int** GetMatrixLocalLinesPtrByRank(int** matrix, int rank, ul local_A_rows, ull n2, ul p1, ul p2) {
    ul diagram_x = rank % p2;
    ul diagram_y = rank % p1;
    ul start_elem_matrix_A = GetNumberLocalMatrixRowsOrCols() * diagram_y * n2;
    ul num_elem_local_A = GetNumberLocalMatrixRowsOrCols() * n2;

    int** sendbuf
    for (ul iter_fill = start_elem_matrix_A; iter_fill < start_elem_matrix_A + num_elem_local_A; ++iter_fill) {

    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    int count_proc;
    ul num_rows_diagram_p1 = atoi(argv[1]);
    ul num_cols_diagram_p2 = atoi(argv[2]);
//------------------------------------------Check-command-prompt-arguments----------------------------------------------

    MPI_Comm_size(MPI_COMM_WORLD, &count_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (num_rows_diagram_p1 * num_cols_diagram_p2 != count_proc) {
        printf("Wrong number of processes. Number of processes must"
               "equal to multiplication of rows and columns at diagram.");
        return 1;
    }
//----------------------------------------------Counting-n1,-n2-and-n3--------------------------------------------------
    ull num_rows_matrix_A_n1, num_cols_matrix_A_rows_matrix_B_n2, num_cols_matrix_B_n3;
    num_rows_matrix_A_n1 = num_rows_diagram_p1 * SIZER;
    num_cols_matrix_A_rows_matrix_B_n2 = SIZER;
    num_cols_matrix_B_n3 = num_cols_diagram_p2 * SIZER;
//----------------------------------------Memory-allocate-and-generate-matrices-----------------------------------------

    ul local_A_rows = GetNumberLocalMatrixRowsOrCols();
    int** local_matrix_A = NULL;
    AllocMatrixMemory(local_matrix_A, local_A_rows, num_cols_matrix_A_rows_matrix_B_n2);

    ul local_B_columns = GetNumberLocalMatrixRowsOrCols();
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
