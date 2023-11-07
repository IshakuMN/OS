
#!/bin/bash

# Compile programs
gcc -o mmu mmu.c
gcc -o os os.c

# Test case 1
./os 4 3 &
OS_PID=$!
./mmu 4 "R0 R0 R1 R0 R1 R2 R0 R1 R2 R3 R0 R1 R2 R0 R1 R0 R0 R0 R1 R0 R1 R2 R0 R1 R2 R3 R0 R1 R2 R0 R1 R0" $OS_PID

# Test case 2  
./os 5 3 &
OS_PID=$!
./mmu 5 "R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 R0 R2 R0 W2 R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4" $OS_PID

# Cleanup
rm mmu os


# Random 
./pager 4 3 random

# NFU
./pager 4 3 nfu 

# Aging
./pager 4 3 aging

./mmu 4 "..." | grep "hit ratio"

# Random
./pager 4 3 random > random.out

# NFU 
./pager 4 3 nfu > nfu.out

# Aging
./pager 4 3 aging > aging.out