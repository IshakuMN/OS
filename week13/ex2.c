#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

// Structure to hold a process state
struct Process
{
    int allocated[MAX_RESOURCES];
    int max[MAX_RESOURCES];
    int need[MAX_RESOURCES];
    bool finished;
};

// Function prototypes
bool checkIfSafeState(struct Process processes[], int numProcesses, int available[]);
bool isLessThanOrEqual(int a[], int b[]);
void printProcessesThatCausedDeadlock(struct Process processes[], int numProcesses);

int main()
{

    int numProcesses;
    int available[MAX_RESOURCES];

    struct Process processes[MAX_PROCESSES];

    // Read input data
    FILE *inFile = fopen("input.txt", "r");

    fscanf(inFile, "%d %d %d", &available[0], &available[1], &available[2]);

    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        fscanf(inFile, "%d %d %d", &processes[i].allocated[0],
               &processes[i].allocated[1], &processes[i].allocated[2]);
    }

    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        fscanf(inFile, "%d %d %d", &processes[i].max[0],
               &processes[i].max[1], &processes[i].max[2]);
    }

    fclose(inFile);

    numProcesses = MAX_PROCESSES;

    // Calculate need matrix
    for (int i = 0; i < numProcesses; i++)
    {
        for (int j = 0; j < MAX_RESOURCES; j++)
        {
            processes[i].need[j] = processes[i].max[j] - processes[i].allocated[j];
        }
    }

    bool safeState = checkIfSafeState(processes, numProcesses, available);

    if (safeState)
    {
        printf("System is in safe state, no deadlock detected\n");
    }
    else
    {
        printProcessesThatCausedDeadlock(processes, numProcesses);
    }

    return 0;
}

// Checks system state using Banker's algorithm
// Returns true if safe, false if deadlock
bool checkIfSafeState(struct Process processes[], int numProcesses, int available[])
{

    bool done[numProcesses];

    for (int i = 0; i < numProcesses; i++)
    {
        done[i] = false;
    }

    int work[MAX_RESOURCES];

    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        work[i] = available[i];
    }

    bool found;

    do
    {
        found = false;
        for (int i = 0; i < numProcesses; i++)
        {
            if (!done[i] && isLessThanOrEqual(processes[i].need, work))
            {
                for (int j = 0; j < MAX_RESOURCES; j++)
                {
                    work[j] += processes[i].allocated[j];
                }
                done[i] = true;
                found = true;
            }
        }
    } while (found);

    for (int i = 0; i < numProcesses; i++)
    {
        if (!done[i])
        {
            return false;
        }
    }
    return true;
}

// Returns true if a <= b for resource vectors
bool isLessThanOrEqual(int a[], int b[])
{
    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        if (a[i] > b[i])
        {
            return false;
        }
    }
    return true;
}

// Prints processes that caused deadlock if system not safe
void printProcessesThatCausedDeadlock(struct Process processes[], int numProcesses)
{

    printf("Processes causing deadlock:");

    for (int i = 0; i < numProcesses; i++)
    {
        if (!processes[i].finished)
        {
            printf(" P%d", i);
        }
    }

    printf("\n");

    return;
}