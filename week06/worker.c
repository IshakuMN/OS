// worker.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>

#define TRI_BASE 1000000

pid_t pid;
int process_idx;
long tris = 0; // Variable to count triangular numbers

// Function to check if a number is triangular
bool is_triangular(int n)
{
    for (int i = 1; i <= n; i++)
    {
        if (i * (i + 1) == 2 * n)
        {
            return true;
        }
    }
    return false;
}

// Function to generate a big random number
long big_n()
{
    time_t t;
    long n = 0;
    srand((unsigned)time(&t));
    while (n < TRI_BASE)
    {
        n += rand();
    }
    return n % TRI_BASE;
}

// Signal handler
void signal_handler(int signum)
{
    if (signum == SIGTSTP)
    {
        // Suspend the process
        kill(getpid(), SIGSTOP);
    }
    else if (signum == SIGCONT)
    {
        // Continue the process
        return;
    }
    else if (signum == SIGTERM)
    {
        // Terminate the process
        printf("Process %d: count of triangulars found so far is %ld\n", process_idx, tris);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s process_idx\n", argv[0]);
        return 1;
    }

    // Parse the process index
    process_idx = atoi(argv[1]);

    // Register signal handlers
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGCONT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // Get a big random number to start with
    long n = big_n();
    printf("Process %d (PID=%d): will find the next triangular number from (%ld, inf)\n", process_idx, getpid(), n);

    while (1)
    {
        if (is_triangular(n))
        {
            tris++;
            printf("Process %d (PID=%d): I found this triangular number %ld\n", process_idx, getpid(), n);
        }
        n++;
    }

    return 0;
}
