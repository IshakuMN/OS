#!/bin/bash

# Compile the C program
gcc -o ex4 ex4.c

# Check for compilation errors
if [ $? -eq 0 ]; then
    # Run the compiled program with the command line argument
    ./ex4 "$1"
    
    # Clean up the compiled executable
    rm ex4
else
    echo "Compilation failed."
fi

#Note: 
# ./ex4 "YourStringHere"
# run the above snippet to get your result from the terminal replacing "YourStringHere" with your string

