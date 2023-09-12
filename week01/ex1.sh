
# Create the directory
mkdir ~/week01

# Navigate to the directory 
cd ~/week01

# Get the last 5 entries containing 'gcc' in reverse alphabetical order 
and save them to a file
ls /usr/bin/ | grep 'gcc' | sort -r | tail -n 5 > ~/week01/ex1.txt

