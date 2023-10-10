#!/bin/bash

# Compile the program with pthread support
gcc -pthread ex2.c -o ex2

# Run the program with a specified number of threads (e.g., 5)
./ex2

# Clean up the executable
rm ex2
