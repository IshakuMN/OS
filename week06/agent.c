#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void sigusr1_handler(int signo)
{
    // Read and print the contents of text.txt
    int fd = open("text.txt", O_RDONLY);
    char buffer[1024];
    ssize_t n;
    while ((n = read(fd, buffer, sizeof(buffer))) > 0)
    {
        write(STDOUT_FILENO, buffer, n);
    }
    close(fd);
}

void sigusr2_handler(int signo)
{
    // Print a termination message and exit
    printf("Process terminating...\n");
    exit(0);
}

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

    // Create and write the PID to /var/run/agent.pid
    pid_t pid = getpid();
    int pid_file = open("/var/run/agent.pid", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (pid_file < 0)
    {
        perror("Failed to open /var/run/agent.pid");
        exit(1);
    }
    dprintf(pid_file, "%d", pid);
    close(pid_file);

    // Set signal handlers
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    // Read and print the initial contents of text.txt
    int text_file = open("text.txt", O_RDONLY);
    if (text_file < 0)
    {
        perror("Failed to open text.txt");
    }
    else
    {
        char buffer[1024];
        ssize_t n;
        while ((n = read(text_file, buffer, sizeof(buffer))) > 0)
        {
            write(STDOUT_FILENO, buffer, n);
        }
        close(text_file);
    }

    while (1)
    {
        sleep(1); // Sleep indefinitely
    }

    return 0;
}
