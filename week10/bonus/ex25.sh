#!/bin/bash


# 1. Make an empty file ex5.txt
echo "Creating an empty file ex5.txt..."
touch ex5.txt

# 2. Remove write permission for everybody
echo "Removing write permission for everybody..."
chmod a-w ex5.txt

# 3. Grant all permissions to owner and others (not group)
echo "Granting all permissions to owner and others (not group)..."
chmod ugo=rwx,g= ex5.txt

# 4. Make group permissions equal to user permissions
echo "Making group permissions equal to user permissions..."
chmod g=u ex5.txt

# 5. Interpretation of permission sets
echo "Interpreting permission sets..."
# 660 - User and group have read and write permissions; others have no permissions
# 775 - User and group have read, write, and execute permissions; others have read and execute permissions
# 777 - Everyone (user, group, and others) has read, write, and execute permissions

# Check current permissions
echo "Current permissions of ex5.txt:"
ls -l ex5.txt

# Set permissions to 660 and check
chmod 660 ex5.txt
echo "Permissions after setting to 660:"
ls -l ex5.txt

# Set permissions to 775 and check
chmod 775 ex5.txt
echo "Permissions after setting to 775:"
ls -l ex5.txt

# Set permissions to 777 and check
chmod 777 ex5.txt
echo "Permissions after setting to 777:"
ls -l ex5.txt
