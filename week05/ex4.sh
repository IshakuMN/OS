#!/bin/bash

# Compile the C program
gcc -o ex4 ex4.c -lpthread

# Define values for n and m
n=10000000
m_values=(1 2 4 10 100)

# Create or truncate the results file
> ex4_res.txt

# Run the program for each value of m and measure execution time
for m in "${m_values[@]}"
do
    echo "Running with n=$n and m=$m"
    
    # Measure execution time and append it to the results file
    { time ./ex4 $n $m; } 2>> ex4_res.txt
done

# Clean up the executable
rm -f ex4
