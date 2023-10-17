
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

ProcessData find_next_process()
{
    int location = -1;
    int min_bt = INT_MAX;

    for (int i = 0; i < data_size; i++)
    {
        if (data[i].at <= total_time && data[i].burst > 0)
        {
            if (data[i].burst < min_bt)
            {
                min_bt = data[i].burst;
                location = i;
            }
        }
    }

    if (location == -1)
    {
        // No process is ready to run; increment the time and retry.
        total_time++;
        return find_next_process();
    }

    return data[location];
}

void report()
{
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;
    for (int i = 0; i < data_size; i++)
    {
        printf("process %d: \n", i);
        printf("  at=%d\n", data[i].at);
        printf("  bt=%d\n", data[i].bt);
        printf("  ct=%d\n", data[i].ct);
        printf("  wt=%d\n", data[i].wt);
        printf("  tat=%d\n", data[i].tat);
        printf("  rt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }

    printf("data size = %d\n", data_size);
    float avg_wt = (float)sum_wt / data_size;
    float avg_tat = (float)sum_tat / data_size;
    printf("Average results for this run:\n");
    printf("  avg_wt=%f\n", avg_wt);
    printf("  avg_tat=%f\n", avg_tat);
}

void check_burst()
{
    for (int i = 0; i < data_size; i++)
        if (data[i].burst > 0)
            return;

    // report simulation results
    report();

    // terminate the scheduler :)
    exit(EXIT_SUCCESS);
}

// This function is called every one second as handler for SIGALRM signal
void schedule_handler(int signum)
{
    // increment the total time
    total_time++;

    if (running_process != -1)
    {
        data[running_process].burst--;
        printf("Scheduler: Runtime: %u seconds\n\n\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

        if (data[running_process].burst == 0)
        {
            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            terminate(ps[running_process]);
            // printf("I am killing process %d\n", running_process);
            waitpid(ps[running_process], NULL, 0);
            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
        }
    }

    check_burst();

    ProcessData next_process = find_next_process();

    // printf("Next process is %d, %d\n", next_process.idx, next_process.burst);
    if (next_process.idx != running_process)
    {
        if (running_process != -1)
        {
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            suspend(ps[running_process]);
        }

        running_process = next_process.idx;
        if (ps[running_process] == 0)
        {
            create_process(running_process);
        }
        else
        {
            printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            resume(ps[running_process]);
        }
        data[running_process].rt = total_time - data[running_process].at;
    }
}

int main(int argc, char *argv[])
{
    // read the data file
    FILE *in_file = fopen(argv[1], "r");
    if (in_file == NULL)
    {
        printf("File is not found or cannot open it!\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        read_file(in_file);
    }

    // set a timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while (1)
        ; // infinite loop
}
