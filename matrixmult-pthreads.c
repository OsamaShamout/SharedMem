#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

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

pthread_mutex_t mutex;

void printWarning(){
    printf("\n******WARNING******\n\nYou are wasting computational resources. \nPlease consider using at maximum the number of threads to be equal to the dimension matrix C (B_cols).\n\n******WARNING******\n\n");
}

void *slave(void *arg)
{
    int thread_id = *(int *)arg;
    int i, j, k;
    int start = thread_id * B_col / num_threads;
    int end = (thread_id + 1) * B_col / num_threads;

    //Check threads debugging
    // printf("The thread is %d start is %d and the end is %d \n", thread_id, start, end);

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

        //Check if matrix dimensions are greater than MAX_DIM.
        if (A_col > MAX_DIM || A_row > MAX_DIM || B_col > MAX_DIM || B_row > MAX_DIM)
        {
            printf("Matrix dimensions cannot be greater than %d\n", MAX_DIM);
            exit(1);
        }
        
        //Tell user if they are wasting computational resources
        if( num_threads > B_col){
            printWarning();
        }

        //Check if matrix multiplication is possible
        if (A_col == 0 || B_row == 0 || A_col != B_row)
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
                A[i][j] = 1;
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
                B[i][j] = 1;
            }
        }

        /* We sat element numbers to be 1 as we can easily raise N*N matrix to the power of 2 (A*B)
        which gives us the matrix C with all elements C(i,j) = N. Thus easily verifable and 
        to debug. 
        Ex: 2x2 matrix A*B = C, C = 2x2 matrix with all elements = 2.
        */

        // // Print matrix B for debugging
        // for (i = 0; i < B_row; i++)
        // {
        //     for (j = 0; j < B_col; j++)
        //     {
        //         printf("%d", B[i][j]);
        //     }
        //     printf("\n");
        // }


        //Matrix C
        for (i = 0; i < A_col; i++)
        {
            for (j = 0; j < B_row; j++)
            {
                C[i][j] = 0;
            }
        }

        // Initialize mutex
        pthread_mutex_init(&mutex, NULL);

        // Initialize threads
        pthread_t threads[num_threads];

        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);

        // Create threads
        for ( i = 0; i < num_threads; i++) {
            //To avoid i being passed to multiple threads and causing a race condition
            //we are dynamically allocating memory for each thread id
           int* thread_id = malloc(sizeof(int));
           *thread_id = i;
           if (pthread_create(&threads[i], NULL, slave, thread_id) != 0) {
               printf("Error creating thread %d", i);
            }
        }

        // Wait for threads to complete and join
        for (i = 0; i < num_threads; i++){
           if(pthread_join(threads[i], NULL) != 0)
                printf("Error in joining thread %d", i);
        }

        gettimeofday(&end_time, NULL);
        
        //Return time in seconds
        double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

        //Print result for debugging
        // printf("Result matrix C:\n");
        // for (i = 0; i < A_row; i++)
        // {
        //     for (j = 0; j < B_col; j++)
        //     {
        //         printf("%d ", C[i][j]);
        //     }
        //     printf("\n");
        // }

        printf("The matrix C is %d x %d executed successfully with number of threads %d\n", A_row, B_col, num_threads);
        printf("Execution time in seconds: %f\n\n", elapsed_time);
        return 0;
    }
}