#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000

int A[N][N];
int B[N][N];
int C[N][N];

void *worker(void *arg)
{
    int thread_id = *(int *)arg;
    int i, j, k;
    int num_threads = *((int *)arg + 1);
    int start = thread_id * N / num_threads;
    int end = (thread_id + 1) * N / num_threads;

    for (i = start; i < end; i++)
    {
        for (j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

//argv[1] = number of threads
//argv[2] = matrix A column size
//argv[3] = matrix B row size

int main(int argc, char **argv)
{
    if(argc != 4){
        printf("Usage: ./matrixmult-pthreads <number of threads> <matrix A cols #> <matrix B rows #>\n");
        exit(1);
    }
    else{
        int NUM_THREADS = atoi(argv[1]);
        int matrix_a_col = atoi(argv[2]);
        int matrix_b_row = atoi(argv[3]);
    
        int i, j;
        int thread_args[NUM_THREADS];

        // Initialize matrices A and B
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                A[i][j] = i + j;
                B[i][j] = i - j;
            }
        }

        // Create threads
        for (i = 0; i < NUM_THREADS; i++)
        {
            thread_args[i] = i;
            pthread_create(&threads[i], NULL, worker, &thread_args[i]);
        }

        // Wait for threads to complete
        for (i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(threads[i], NULL);
        }

        // Print result
        printf("Result matrix C:\n");
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }

        return 0;
    }
}