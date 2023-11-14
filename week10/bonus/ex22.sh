#!/bin/bash

# Example 2 Implementation

# 1. Check inode, block count, block size, total size, and permissions of ex1
echo "Checking details of ex1..."
stat ex1
# The output of this will show the inode number, block size, number of blocks, total size, and permissions

# 2. Copy ex1 to ex2 and check the number of links for ex2
echo "Copying ex1 to ex2..."
cp ex1 ex2

echo "Checking details of ex2..."
stat ex2
# This will show if ex1 and ex2 have the same inode number (they should not) and the number of links

# 3. Identify files with 3 links in /etc
echo "Finding files with 3 links in /etc..."
stat -c "%h - %n" /etc/* | grep ^3
# This command lists files in /etc with 3 links. The number 3 represents the count of hard links to the file.
