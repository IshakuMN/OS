#!/bin/bash

# Example 3 Implementation

# 1. Create gen.sh script
echo "Creating gen.sh script..."
echo 'for (( i=0; i<$1; i++ )); do echo $RANDOM >> $2; done' > gen.sh
chmod +x gen.sh

# 2. Create ex1.txt using gen.sh
echo "Generating ex1.txt with 10 random numbers..."
./gen.sh 10 ex1.txt

# 3. Create hard links to ex1.txt
echo "Creating hard links ex11.txt and ex12.txt..."
ln ex1.txt ex11.txt
ln ex1.txt ex12.txt

# 4. Compare contents of all files
echo "Comparing contents of ex1.txt, ex11.txt, and ex12.txt..."
diff ex1.txt ex11.txt
diff ex1.txt ex12.txt

# 5. Check inode numbers and save output
echo "Checking inode numbers..."
stat ex1.txt ex11.txt ex12.txt > output.txt

# 6. Check disk usage of ex1.txt
echo "Checking disk usage of ex1.txt..."
du ex1.txt

# 7. Create another hard link and move to /tmp
echo "Creating hard link ex13.txt and moving it to /tmp..."
ln ex1.txt ex13.txt
mv ex13.txt /tmp

# Trace all links to ex1.txt
echo "Tracing all links to ex1.txt in current and root paths..."
find . -inum $(stat -c "%i" ex1.txt)
find / -inum $(stat -c "%i" ex1.txt)

# 8. Check number of hard links and remove all links
echo "Checking number of hard links and removing them..."
find . -samefile ex1.txt -exec rm {} \;
