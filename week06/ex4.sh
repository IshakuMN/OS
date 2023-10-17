#!/bin/bash

# Compile the scheduler_rr.c program
gcc scheduler_rr.c -o scheduler_rr

# Run the scheduler_rr program with your input file and specify the quantum
./scheduler_rr data.txt 3

# Save the output to ex4.txt
#./scheduler_rr data.txt > ex4.txt
