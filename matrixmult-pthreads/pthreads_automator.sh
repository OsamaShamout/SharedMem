#!/bin/bash
# This script will run the matrixmult-pthreads program with 2-16 threads
# and output the results to a file called pthreads.txt
# regex it with the trivial: \n?The matrix C is \d+ x \d+ executed successfully with number of threads \d+\nExecution time in seconds:\s*([\d.]+)
# and replace with $1 then place in excel file

output="pthreads.txt"
if [ ! -f "$output" ]; then
    touch "$output"
fi

for ((j=1; j<=11; j+=1)); do
    for((i=500; i<=3500; i+=500)); do
        ./matrixmult-pthreads "$j" "$i" "$i" "$i" "$i"  >> "$output" 2>&1
    done
done

for each thread t:
    compute start and end column indices for thread t

for each thread t:
    create a new thread that runs the slave function with argument t

for each thread t:
    wait for thread t to finish execution

slave function:
    for each column j assigned to the thread:
        for each row i:
            compute C[i][j] += A[i][k] * B[k][j]
            lock mutex
            update C[i][j]
            unlock mutex