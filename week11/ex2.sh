#!/bin/bash

# Script to compile and run file system programs

# Define the output executables
CREATE_FS_OUTPUT="create_fs"
EX2_OUTPUT="ex2"

# Compile create_fs.c and set output as create_fs
gcc create_fs.c -o $CREATE_FS_OUTPUT
gcc_status_1=$?

# Compile ex2.c and set output as ex2
gcc ex2.c -o $EX2_OUTPUT
gcc_status_2=$?

# Verify both compilations were successful
if [[ $gcc_status_1 -eq 0 ]] && [[ $gcc_status_2 -eq 0 ]]; then
    echo "Both programs compiled successfully"

    # Initialize the file system
    ./$CREATE_FS_OUTPUT disk0

    # Execute the main program with provided input
    ./$EX2_OUTPUT input.txt
else
    echo "Error in compilation process"
fi
