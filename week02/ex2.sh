#!/bin/bash

gcc -o ex2 ex2.c

if [ $? -eq 0 ]; then
    echo "Compilation successful."
    ./ex2
    rm ex2
else
    echo "Compilation failed."
fi


