#!/bin/bash

# Example 1 Implementation

# 1. Check file types in /dev and /etc
echo "Checking file types in /dev and /etc..."
file /dev/* /etc/*  # This might generate a lot of output

# 2. Count the number of directories in /etc
echo "Counting number of directories in /etc..."
ls -l /etc | grep ^d | wc -l

# 3. Write a hello world program in C
echo "Writing hello world program in C..."
cat <<EOF > ex1.c
#include <stdio.h>

int main() {
    printf("Hello, world!\\n");
    return 0;
}
EOF

# Check file type before compilation
echo "Checking file type of ex1.c before compilation..."
file ex1.c

# Compile the program
echo "Compiling ex1.c..."
gcc ex1.c -o ex1

# Check file type after compilation
echo "Checking file type of ex1 after compilation..."
file ex1

# 4. Modify the program to print "Привет, мир!"
echo "Modifying the program to print 'Привет, мир!'..."
sed -i 's/Hello, world!/Привет, мир!/' ex1.c

# Check file type of the modified program
echo "Checking file type of modified ex1.c..."
file ex1.c

# Compile the modified program
echo "Compiling the modified ex1.c..."
gcc ex1.c -o ex1

# Check file type of the compiled modified program
echo "Checking file type of compiled modified ex1..."
file ex1
