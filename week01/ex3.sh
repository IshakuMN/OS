#!/bin/bash

# Print the date and wait for 3 seconds
date
sleep 3

# Create the first directory and navigate into it
mkdir dir1
cd dir1

# List the items of the root directory, sorted by time (oldest first) and 
save to root.txt
ls -lt / | tail -r > root.txt

# Print the content of root.txt
cat root.txt

# Go back to the previous directory
cd ..

# Print the date and wait for 3 seconds
date
sleep 3

# Create the second directory and navigate into it
mkdir dir2
cd dir2

# List the items of the home directory, sorted by time (oldest first) and 
save to home.txt
ls -lt ~ | tail -r > home.txt

# Print the content of home.txt
cat home.txt

# Go back to the previous directory
cd ..

# Print the date and wait for 3 seconds
date
sleep 3

# Display items of the new directories
echo "Contents of dir1:"
ls dir1

echo "Contents of dir2:"
ls dir2
