#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    // Create the directory if it doesn't exist
    if (access("/var/run", F_OK) != 0)
    {
        if (mkdir("/var/run", 0755) != 0)
        {
            perror("Failed to create /var/run directory");
            exit(1);
        }
    }

    // Check if the agent is running by reading its PID from /var/run/agent.pid
    int pid_file = open("/var/run/agent.pid", O_RDONLY);
    if (pid_file < 0)
    {
        perror("Error: No agent found.");
        exit(1);
    }

    char pid_str[32];
    ssize_t n = read(pid_file, pid_str, sizeof(pid_str));
    close(pid_file);

    if (n < 0)
    {
        perror("Failed to read agent's PID");
        exit(1);
    }

    pid_str[n] = '\0';
    pid_t agent_pid = atoi(pid_str);

    printf("Agent found.\n");

    while (1)
    {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"} to send to the agent: ");
        char command[20];
        scanf("%s", command);

        if (strcmp(command, "read") == 0)
        {
            kill(agent_pid, SIGUSR1);
        }
        else if (strcmp(command, "exit") == 0)
        {
            kill(agent_pid, SIGUSR2);
            exit(0);
        }
        else if (strcmp(command, "stop") == 0)
        {
            kill(agent_pid, SIGSTOP);
        }
        else if (strcmp(command, "continue") == 0)
        {
            kill(agent_pid, SIGCONT);
        }
        else
        {
            printf("Invalid command.\n");
        }
    }

    return 0;
}
