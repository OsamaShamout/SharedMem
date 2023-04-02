#!/bin/bash
# This script will run the matrixmult-pthreads program with 2-16 threads
# and output the results to a file called pthreads.txt
# regex it with the trivial: \n?The matrix C is \d+ x \d+ executed successfully with number of threads \d+\nExecution time in seconds:\s*([\d.]+)
# and replace with $1 then place in excel file

output="pthreads.txt"
if [ ! -f "$output" ]; then
    touch "$output"
fi

for ((j=1; j<=12; j+=1)); do
    for((i=500; i<=4000; i+=500)); do
        ./matrixmult-pthreads "$j" "$i" "$i" "$i" "$i"  >> "$output" 2>&1
    done
done

