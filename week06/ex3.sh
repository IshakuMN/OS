#!/bin/bash

# Compile the sjf.c program
gcc sjf.c -o sjf

# Run the sjf program with the input file (replace 'input.txt' with your input file)
./sjf data.txt

# Clean up the compiled binary
rm sjf
