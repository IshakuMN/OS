#!/bin/bash


# 1. Delete ./tmp folder if it exists and create a symbolic link tmp1
echo "Setting up the environment..."
[ -d "./tmp" ] && rm -rf ./tmp
ln -s ./tmp tmp1

# 2. Run ls -li before creating ./tmp
echo "Running ls -li before creating ./tmp..."
ls -li

# 3. Create the folder ./tmp
echo "Creating ./tmp directory..."
mkdir ./tmp

# 4. Run ls -li after creating ./tmp
echo "Running ls -li after creating ./tmp..."
ls -li

# 5. Create a file ex1.txt using gen.sh and add it to ./tmp
echo "Creating ex1.txt and adding it to ./tmp..."
./gen.sh 10 ./tmp/ex1.txt
echo "Checking ./tmp1 contents..."
ls -li ./tmp1

# 6. Create a symbolic link within tmp1 to ./tmp and add another ex1.txt
echo "Creating symbolic link within tmp1 to ./tmp..."
ln -s $(pwd)/tmp ./tmp1/tmp2
echo "Creating another ex1.txt and adding it to ./tmp1/tmp2..."
./gen.sh 10 ./tmp1/tmp2/ex1.txt

# 7. Check the content of sub-folders
echo "Checking content of sub-folders..."
ls -liR ./tmp1

# 8. Access the nested symbolic links
echo "Accessing nested symbolic links..."
ls -li ./tmp1/tmp2/tmp2/tmp2  # This might result in an error due to recursive links

# 9. Delete ./tmp and check symbolic links
echo "Deleting ./tmp and checking symbolic links..."
rm -rf ./tmp
ls -li tmp1

# 10. Delete all other symbolic links
echo "Cleaning up symbolic links..."
rm -f tmp1
