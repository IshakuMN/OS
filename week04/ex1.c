#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    // Get the start time
    clock_t start_time = clock();

    // Print the main process ID and parent ID
    printf("Main Process: PID = %d\n", getpid());

    // Create the first child process
    pid_t child1_pid = fork();

    if (child1_pid == -1)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child1_pid == 0)
    {
        // This is the child process
        // Print child 1's process ID and parent ID
        printf("Child 1: PID = %d, Parent PID = %d\n", getpid(), getppid());

        // Add your child 1 code here

        // Calculate and print execution time for child 1
        clock_t end_time = clock();
        double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
        printf("Child 1 Execution Time: %.2f ms\n", execution_time);

        exit(EXIT_SUCCESS);
    }
    else
    {
        // This is the parent process
        // Create the second child process
        pid_t child2_pid = fork();

        if (child2_pid == -1)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (child2_pid == 0)
        {
            // This is child process 2
            // Print child 2's process ID and parent ID
            printf("Child 2: PID = %d, Parent PID = %d\n", getpid(), getppid());

            // Add your child 2 code here

            // Calculate and print execution time for child 2
            clock_t end_time = clock();
            double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
            printf("Child 2 Execution Time: %.2f ms\n", execution_time);

            exit(EXIT_SUCCESS);
        }
        else
        {
            // This is the parent process
            // Wait for both child processes to finish
            wait(NULL);
            wait(NULL);

            // Calculate and print execution time for the main process
            clock_t end_time = clock();
            double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
            printf("Main Process Execution Time: %.2f ms\n", execution_time);

            exit(EXIT_SUCCESS);
        }
    }
}
