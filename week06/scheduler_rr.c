#include <sys/wait.h>
#include <sys/time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#define PS_MAX 10

// holds the scheduling data of one process
typedef struct
{
    int idx;
    int at, bt, rt, wt, ct, tat;
    int burst;
    int remaining_quantum; // remaining time slice of the quantum
} ProcessData;

// the idx of the running process
int running_process = -1;
// the total time of the timer
unsigned total_time;
// data of the processes
ProcessData data[PS_MAX];

// array of all process pids
pid_t ps[PS_MAX];

// size of data array
unsigned data_size;

int quantum; // quantum specified by the user

void read_file(FILE *file)
{
    data_size = 0;
    int idx, at, bt;
    int line = 0;
    fscanf(file, "%*s %*s %*s");
    while (fscanf(file, "%d %d %d", &idx, &at, &bt) == 3)
    {
        data[data_size].idx = idx;
        data[data_size].at = at;
        data[data_size].bt = bt;
        data[data_size].rt = bt;
        data[data_size].wt = 0;
        data[data_size].ct = 0;
        data[data_size].tat = 0;
        data[data_size].burst = bt;
        data[data_size].remaining_quantum = quantum;
        data_size++;
    }

    printf("The data.size is, %d\n", data_size);
    // TODO: extract the data of processes from the {file}
}

// send signal SIGCONT to the worker process
void resume(pid_t process)
{
    if (process > 0)
    {
        kill(process, SIGCONT);
    }
    // TODO: send signal SIGCONT to the worker process if it is not in one of the states
}

// send signal SIGTSTP to the worker process
void suspend(pid_t process)
{
    if (process > 0)
    {
        kill(process, SIGTSTP);
    }
    // TODO: send signal SIGTSTP to the worker process if it is not in one of the states
}

// send signal SIGTERM to the worker process
void terminate(pid_t process)
{
    if (process > 0)
    {

        kill(process, SIGTERM);
    }
    // TODO: send signal SIGTERM to the worker process if it is not in one of the states
}

// create a process using fork
void create_process(int new_process)
{
    if (running_process != -1)
    {
        // Stop the running process
        suspend(ps[running_process]);
    }

    pid_t child_pid = fork();

    if (child_pid == 0)
    {
        char process_idx_str[10];
        sprintf(process_idx_str, "%d", new_process);
        execlp("./worker", "./worker", process_idx_str, (char *)NULL);
        perror("exec failed");
        exit(EXIT_FAILURE);
    }
    else if (child_pid > 0)
    {
        ps[new_process] = child_pid;
        running_process = new_process;
        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", new_process, data[new_process].burst);
    }
    else
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    // TODO: stop the running process

    // TODO: fork a new process and add it to ps array

    // TODO: Now the idx of the running process is new_process

    // TODO: The scheduler process runs the program "./worker {new_process}"
}

// find next process for running
ProcessData find_next_process()
{
    // location of next process in {data} array
    int location = running_process;

    do
    {
        location = (location + 1) % data_size;
        if (data[location].burst > 0)
        {
            return data[location];
        }
    } while (location != running_process);

    // If no process with burst time > 0 is found, return the current running process
    return data[running_process];
}

// reports the metrics and simulation results
void report()
{
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;

    for (int i = 0; i < data_size; i++)
    {
        printf("Process %d: AT: %d, BT: %d, CT: %d, TAT: %d, WT: %d\n",
               data[i].idx, data[i].at, data[i].bt, data[i].ct, data[i].tat, data[i].wt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }
    printf("Average Turnaround Time: %.2f\n", (float)sum_tat / data_size);
    printf("Average Waiting Time: %.2f\n", (float)sum_wt / data_size);
}

void schedule_handler(int signum)
{
    if (running_process != -1)
    {
        // Decrease the remaining quantum for the current running process
        data[running_process].remaining_quantum--;

        // If the remaining quantum is zero, suspend the current process and find the next process
        if (data[running_process].remaining_quantum == 0)
        {
            suspend(ps[running_process]);
            data[running_process].remaining_quantum = quantum;
            ProcessData next_process = find_next_process();
            running_process = next_process.idx;
            resume(ps[running_process]);
            printf("Scheduler: Switching to Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./scheduler <input_file> <quantum>\n");
        return 1;
    }

    char *input_file = argv[1];
    FILE *file = fopen(input_file, "r");
    if (file == NULL)
    {
        perror("Failed to open input file");
        return 1;
    }

    quantum = atoi(argv[2]);

    read_file(file);
    fclose(file);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &schedule_handler;
    sigaction(SIGALRM, &sa, NULL);

    // Set up the timer
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    // Start the first process
    create_process(0);

    while (1)
    {
        // Wait for child process to change state
        int status;
        pid_t pid = wait(&status);

        if (pid > 0)
        {
            // Find the index of the completed process
            int completed_process;
            for (completed_process = 0; completed_process < data_size; completed_process++)
            {
                if (ps[completed_process] == pid)
                {
                    break;
                }
            }

            // Update the process data
            data[completed_process].ct = total_time;
            data[completed_process].tat = data[completed_process].ct - data[completed_process].at;
            data[completed_process].wt = data[completed_process].tat - data[completed_process].bt;

            printf("Scheduler: Process %d completed (Turnaround Time: %d, Waiting Time: %d)\n",
                   completed_process, data[completed_process].tat, data[completed_process].wt);

            // Check if all processes are completed
            int all_completed = 1;
            for (int i = 0; i < data_size; i++)
            {
                if (data[i].burst > 0)
                {
                    all_completed = 0;
                    break;
                }
            }

            // If all processes are completed, report the results and exit the program
            if (all_completed)
            {
                report();
                exit(0);
            }
            else
            {
                // Find the next process and resume it
                ProcessData next_process = find_next_process();
                running_process = next_process.idx;
                resume(ps[running_process]);
                printf("Scheduler: Switching to Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            }
        }
    }

    return 0;
}