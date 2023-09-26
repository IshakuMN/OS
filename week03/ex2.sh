#!/bin/bash

# Compile the C program
gcc -o ex2 ex2.c -lm

# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run the compiled program
    ./ex2
else
    echo "Compilation failed. Please check your code."
fi
