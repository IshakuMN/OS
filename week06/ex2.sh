# #!/bin/bash
gcc scheduler.c -o scheduler
gcc worker.c -o worker

./scheduler data.txt

# # Compile the worker program
# gcc worker.c -o worker

# # Run the scheduler with data.txt as the input
# ./scheduler data.txt &

# # Wait for a short period to allow the scheduler to start
# sleep 1

# # Run multiple worker processes in the background
# ./worker 0 &
# ./worker 1 &

# ./worker 2 &
# ./worker 3 &
# # Add more worker processes as needed

# # Wait for all worker processes to finish
# wait

# # Cleanup
# rm worker
