#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define MAX_MESSAGE_SIZE 1024

int main()
{
    int pipe_fd[2];
    pid_t pid;

    // Create an unnamed pipe
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    if ((pid = fork()) == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // Child process (Subscriber)
        close(pipe_fd[1]); // Close the write end of the pipe (not needed for reading)

        char message[MAX_MESSAGE_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = read(pipe_fd[0], message, MAX_MESSAGE_SIZE)) > 0)
        {
            // Print the received message
            write(STDOUT_FILENO, message, bytes_read);
        }

        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent process (Publisher)
        close(pipe_fd[0]); // Close the read end of the pipe (not needed for writing)

        char message[MAX_MESSAGE_SIZE];
        ssize_t bytes_read;

        printf("Enter a message (Ctrl+D to exit):\n");

        while ((bytes_read = read(STDIN_FILENO, message, MAX_MESSAGE_SIZE)) > 0)
        {
            // Send the message to the subscriber
            write(pipe_fd[1], message, bytes_read);
        }

        close(pipe_fd[1]); // Close the write end of the pipe
        wait(NULL);        // Wait for the child process to finish
        exit(EXIT_SUCCESS);
    }

    return 0;
}
