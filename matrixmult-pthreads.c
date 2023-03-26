#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Since we are using static having > 1000 dimension would better involve a dynamic allocation.
// But for the sake of this assignment we are using static allocation.
#define MAX_DIM 1000

int A[MAX_DIM][MAX_DIM];
int B[MAX_DIM][MAX_DIM];
int C[MAX_DIM][MAX_DIM];

int num_threads;
int A_row;
int A_col;
int B_row;
int B_col;

double computation_time;

pthread_mutex_t mutex;

void *slave(void *arg)
{
    clock_t start_time = clock();
    int thread_id = *(int *)arg;
    int i, j, k;
    int start = thread_id * B_col / num_threads;
    int end = (thread_id + 1) * B_col / num_threads;

    // //Check threads debugging
    // printf("Hello from thread %d\n", thread_id);

    for (j = start; j < end; j++)
    {
        for (i = 0; i < A_row; i++)
        {
            int sum = 0;
            for (k = 0; k < A_col; k++)
            {   
                sum += A[i][k] * B[k][j];
            }
            // Lock mutex before updating C (Critical Section) and unlock after updating C
            // Outside loop to avoid unnecessary locking and unlocking
            pthread_mutex_lock(&mutex);
            C[i][j] += sum;
            pthread_mutex_unlock(&mutex);
        }
    }
    clock_t end_time = clock();
    computation_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{   
    if (argc != 6)
    {
        printf("Usage: ./matrixmult-pthreads <number of threads> <matrix A # rows> <matrix A # cols> <matrix B # rows> <matrix B # cols>\n");
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
        

        //Check if matrix multiplication is possible
        if (A_col != B_row)
        {
            printf("Matrix multiplication is not possible\n");
            exit(1);
        }

        int i,j;
        // Initialize matrix A
        for ( i = 0; i < A_row ; i++)
        {
            for ( j = 0; j < A_col ; j++)
            {
                A[i][j] = i + j;
            }
        }

        // // Print matrix A for debugging
        // for (i = 0; i < A_row; i++)
        // {
        //     for (j = 0; j < A_col; j++)
        //     {
        //         printf("%d", A[i][j]);
        //     }
        //     printf("\n");
        // }

        // Initialize matrix B
        for ( i = 0; i < B_row; i++)
        {
            for ( j = 0; j < B_col; j++)
            {
                B[i][j] = i + j;
            }
        }

        // // Print matrix B for debugging
        // for (i = 0; i < B_row; i++)
        // {
        //     for (j = 0; j < B_col; j++)
        //     {
        //         printf("%d", B[i][j]);
        //     }
        //     printf("\n");
        // }

        // Initialize mutex
        pthread_mutex_init(&mutex, NULL);

        // Initialize threads
        pthread_t threads[num_threads];

        // Create threads
        for ( i = 0; i < num_threads; i++)
            if(pthread_create(&threads[i], NULL, slave, &i) != 0)
                printf("Error creating thread %d", i);

        // Wait for threads to complete and join
        for (i = 0; i < num_threads; i++)
           if(pthread_join(threads[i], NULL) != 0)
                printf("Error in joining thread %d", i);

        // // Print result for debugging
        // printf("Result matrix C:\n");
        // for (i = 0; i < A_row; i++)
        // {
        //     for (j = 0; j < B_col; j++)
        //     {
        //         printf("%d ", C[i][j]);
        //     }
        //     printf("\n");
        // }

        printf("\nComputation time: %f seconds", computation_time);

        return 0;
    }
}