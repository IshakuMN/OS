#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0)
    {
        fprintf(stderr, "n should be a positive integer\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        pid_t child_pid = fork();

        if (child_pid == -1)
        {
            perror("fork");
            return 1;
        }

        if (child_pid == 0)
        {
            // This is the child process
            printf("Child process %d created\n", getpid());
            sleep(5);
            exit(0);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < n; i++)
    {
        wait(NULL);
    }

    return 0;
}
