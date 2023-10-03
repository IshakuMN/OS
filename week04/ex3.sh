#!/bin/bash

# Compile the ex3.c program
gcc ex3.c -o ex3

# Run the program for n=3
./ex3 3 &

# Run "ps" with appropriate options to display the process tree
for i in {1..5}; do
    ps axjf
    sleep 5
done

# Run the program for n=5
./ex3 5 &

# Run "ps" with appropriate options to display the process tree
for i in {1..5}; do
    ps axjf
    sleep 5
done

# Clean up the compiled program
rm ex3
