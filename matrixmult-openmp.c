#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <time.h>

// Since we are using static having > 10000 dimension would better involve a dynamic allocation.
// But for the sake of this assignment we are using static allocation.
#define MAX_DIM 10000

int A[MAX_DIM][MAX_DIM];
int B[MAX_DIM][MAX_DIM];
int C[MAX_DIM][MAX_DIM];

int num_threads;
int A_row;
int A_col;
int B_row;
int B_col;

clock_t start_time;
clock_t end_time;
double total_time;

int main(int argc, char **argv) {
    if (argc != 6)
    {
    printf("Usage: ./matrixmult-openmp <number of threads> <matrix A # rows> <matrix A # cols> <matrix B # rows> <matrix B # cols>\n");
    exit(1);
    }
    else
    {

    // Get command line arguments
    num_threads = atoi(argv[1]);
    A_row = atoi(argv[2]);
    A_col = atoi(argv[3]);
    B_row = atoi(argv[4]);
    B_col = atoi(argv[5]);

    if (A_col > MAX_DIM || A_row > MAX_DIM || B_col > MAX_DIM || B_row > MAX_DIM)
    {
        printf("Matrix dimensions cannot be greater than %d\n", MAX_DIM);
        exit(1);
    }

    // Check if matrix multiplication is possible
    if (A_col == 0 || B_row == 0 || A_col != B_row)
    {
        printf("Matrix multiplication is not possible\n");
        exit(1);
    }

    //Matrix initalizations
    int i, j, k;

    // Initialize matrix A
    for (i = 0; i < A_row; i++)
    {
        for (j = 0; j < A_col; j++)
        {
            A[i][j] = 1;
        }
    }

    // Print matrix A for debugging
    for (i = 0; i < A_row; i++)
    {
        for (j = 0; j < A_col; j++)
        {
            printf("%d", A[i][j]);
        }
        printf("\n");
    }

    // Initialize matrix B
    for (i = 0; i < B_row; i++)
    {
        for (j = 0; j < B_col; j++)
        {
            B[i][j] = 1;
        }
    }

    // Print matrix B for debugging
    for (i = 0; i < B_row; i++)
    {
        for (j = 0; j < B_row; j++)
        {
            printf("%d", B[i][j]);
        }
        printf("\n");
    }
    
    // Matrix multiplication using Open MP
  
    omp_set_num_threads(num_threads);
    
    start_time = omp_get_wtime();
    double sum = 0;
    #pragma omp parallel for private(i, j, k, sum) shared(A, B, C) reduction(+:sum)
    for (i = 0; i < A_row; ++i)
    {
        for (j = 0; j < B_col; j++)
        {
            for (k = 0; k < A_col; k++)
            {
               sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    total_time = omp_get_wtime() - start_time;

    printf("\nTotal time: %f seconds\n", total_time);

    printf("The result matrix is:\n");

    for (i= 0; i< N; i++)
    {
        for (j= 0; j< N; j++)
        {
            printf("%d\t",C[i][j]);
        }
        printf("\n");
    }
}
}