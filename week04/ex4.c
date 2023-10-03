#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

void execute_command(char *command)
{
    char *args[64]; // Maximum 64 arguments
    int i = 0;

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child_pid == 0)
    {
        // This is the child process
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        // This is the parent process
        int status;
        waitpid(child_pid, &status, 0);
    }
}

int main()
{
    char input[MAX_INPUT_SIZE];

    while (1)
    {
        printf("Enter a command: ");
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
        {
            break;
        }

        // Remove newline character if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
        }

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        execute_command(input);
    }

    return 0;
}
