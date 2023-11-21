#!/bin/bash

# Step 1: Create a file lofs.img not less than 50 MiB
dd if=/dev/zero of=lofs.img bs=1M count=50

# Step 2: Setup a loop device on the created file
sudo losetup -fP lofs.img

# Find the loop device associated with lofs.img
LOOP_DEVICE=$(sudo losetup -l | grep lofs.img | awk '{print $1}')

# Step 3: Create a Loop File System (LOFS) ext4 on the created file
sudo mkfs.ext4 $LOOP_DEVICE

# Step 4: Create a new directory and mount the filesystem
mkdir -p ./lofsdisk
sudo mount $LOOP_DEVICE ./lofsdisk

# Step 5: Add two files file1 and file2 to the LOFS
echo "FirstName" | sudo tee ./lofsdisk/file1
echo "LastName" | sudo tee ./lofsdisk/file2

# Step 6: Define a function to return paths of all shared libraries
get_libs() {
    ldd $1 | awk 'NF == 4 {print $3}' || true
}

# Step 7: Copy necessary binaries and their shared libraries to LOFS
for BIN in bash cat echo ls; do
    BIN_PATH=$(which $BIN)
    sudo cp $BIN_PATH ./lofsdisk
    for LIB in $(get_libs $BIN_PATH); do
        LIB_DIR=$(dirname $LIB)
        sudo mkdir -p ./lofsdisk$LIB_DIR
        sudo cp $LIB ./lofsdisk$LIB
    done
done

# Step 8: Chroot and run the C program
sudo chroot ./lofsdisk /bin/bash -c "./ex1 > ex1.txt"

# Step 9: Run the program again without chroot
./ex1 >> ./lofsdisk/ex1.txt

# Step 10: Add findings to ex1.txt
echo "Findings: ..." | sudo tee -a ./lofsdisk/ex1.txt
