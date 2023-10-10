#!/bin/bash

# Compile the C program
gcc -o ex3 ex3.c -lpthread

# Define values for n and m
n=10000000
m_values=(1 2 4 10 100)

# Run the program for each value of m and measure execution time
for m in "${m_values[@]}"
do
    echo "Running with n=$n and m=$m"
    time ./ex3 $n $m
done

# Clean up the executable
rm -f ex3
